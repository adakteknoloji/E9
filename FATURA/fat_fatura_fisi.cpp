#include <QMessageBox>
#include <QLCDNumber>
#include "adak_sql.h"
#include "adak_utils.h"
#include "adak_std_utils.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "stok_struct.h"
#include "ui_fat_fatura_fisi.h"
#include "fat_fatura_fisi_class.h"
#include "fat_struct.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "cari_console_utils.h"
#include "cari_fis_utils.h"
#include "muh_fis_utils.h"
#include "sube_console_utils.h"
#include "e9_yetki.h"
#include "e9_log.h"
#include "fat_fis_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "stok_fis_utils.h"
#include "cari_kart_arama_open.h"
#include "fat_open.h"
#include "stok_open.h"
#include "sube_depo_secimi_open.h"
#include "fat_console_utils.h"
#include "fat_enum.h"
#include "fat_gui_utils.h"
#include "dvz_gui_utils.h"
#include "stok_gui_utils.h"
#include "stok_console_utils.h"
#include "sube_struct.h"
#include "sube_enum.h"
#include "irs_console_utils.h"
#include "stok_urun_parti_girisi_batch_open.h"
#include "stok_gelen_seri_no_batch_open.h"
#include "stok_giden_seri_no_secimi_open.h"
#include "stok_giden_parti_no_secimi_open.h"
#include "stok_enum.h"
#include "cari_struct.h"
#include "e9_uretilen_fis_secimi_open.h"
#include "fat_irsaliye_arama_open.h"
#include "cari_oto_ekstre_arama_open.h"

extern ADAK_SQL *              DB;

#define     ROW_ID_COLUMN                           0
#define     FIS_ID_COLUMN                           1
#define     ORDER_COLUMN                            2
#define     URUN_ID_COLUMN                          3
#define     STOK_TAKIP_SEKLI_COLUMN                 4
#define     PARTI_ID_COLUMN                         5
#define     IRSALIYE_SATIRI_ID_COLUMN               6
#define     IADE_EDILEN_FAT_SATIR_ID_COLUMN         7
#define     FIYAT_IRSALIYEDEN_ALINDI_COLUMN         8
#define     SERI_NO_LIST_COLUMN                     9
#define     IRSALIYE_SATIRI_SECIMI_COLUMN           10
#define     FATURA_SATIRI_SECIMI_COLUMN             11
#define     URUN_KODU_COLUMN                        12
#define     URUN_ADI_COLUMN                         13
#define     SERI_PARTI_NO_KODU_COLUMN               14
#define     SATIRDAKI_URUN_MIKTARI_COLUMN           15
#define     TEMEL_BIRIM_KATSAYISI_COLUMN            16
#define     URUN_BIRIMI_COLUMN                      17
#define     ISK_ONCESI_BIRIM_FIYAT_COLUMN           18
#define     ISKONTO_YUZDESI_COLUMN                  19
#define     ISKONTO_TUTARI_COLUMN                   20
#define     ISK_SONRASI_BIRIM_FIYAT_COLUMN          21
#define     TOPLU_ISKONTO_YUZDESI_COLUMN            22
#define     T_IS_SONRASI_BIRIM_FIYAT_COLUMN         23
#define     T_IS_SONRASI_SATIR_TUTARI_COLUMN        24
#define     T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN   25
#define     T_IS_SONRASI_OTV_TUTARI_COLUMN          26
#define     T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN     27
#define     T_IS_SONRASI_KDV_TUTARI_COLUMN          28
#define     T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN     29
#define     KDV_DAHIL_MI_COLUMN                     30
#define     KDV_ORANI_COLUMN                        31
#define     OTV_ORANI_COLUMN                        32
#define     SATIR_TUTARI_COLUMN                     33
#define     KDV_TUTARI_COLUMN                       34
#define     KDV_DAHIL_TUTAR_COLUMN                  35
#define     VERGI_HARIC_TUTAR_COLUMN                36
#define     FATURA_PARA_BIRIMI_COLUMN               37
#define     OTV_TUTARI_COLUMN                       38
#define     OTV_DAHIL_TUTAR_COLUMN                  39


/**************************************************************************************
                   OPEN_FATURA_FISI
***************************************************************************************/

void OPEN_FATURA_FISI( int p_fatura_turu, int p_fis_id, int p_iade_faturasi_mi, QWidget * p_parent )
{
    switch ( p_fatura_turu ) {

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
            if ( SUBE_DEPO_ISLEM_YETKI_KONTROLU ( p_parent ) EQ false ) {
                return;
            }
            break;

        default :
            break;
    }

    FAT_FATURA_FISI * F = new FAT_FATURA_FISI ( p_fatura_turu, p_fis_id, p_iade_faturasi_mi, p_parent );
    F->SHOW( FULL_SCREEN );
}

/**************************************************************************************
                   FAT_FATURA_FISI::FAT_FATURA_FISI
***************************************************************************************/

FAT_FATURA_FISI::FAT_FATURA_FISI ( int p_fatura_turu, int p_fis_id, int p_iade_faturasi_mi, QWidget * p_parent )
:FIS_KERNEL ( p_parent ), m_ui ( new Ui::FAT_FATURA_FISI )
{
    m_fatura_turu                            = p_fatura_turu;
    m_fis_id                                 = p_fis_id;
    m_iade_faturasi_mi                       = p_iade_faturasi_mi;

    m_ui->setupUi   ( this );
    START_FIS_KERNEL( this, DB );
 }

/**************************************************************************************
                   FAT_FATURA_FISI::SETUP_FORM
***************************************************************************************/

void FAT_FATURA_FISI::SETUP_FORM()
{
    SET_HELP_PAGE ("fatura_fisi.html");
    REGISTER_BUTTONS_WIDGET  ( m_ui->navigation_button_widget );

    M_FIS                                    =   new FATURA_FISI_STRUCT;
    M_FIS_SATIRI                             =   new FATURA_FIS_SATIRI_STRUCT;

    M_FIS->KDV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    M_FIS->KDV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    M_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

    M_FIS->OTV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
    M_FIS->OTV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
    M_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

    M_FIS->TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY =   new int     [KDV_OTV_ORANLARI_SAYISI];

    //! FATURA TURU DISABLE HALE GETIRILIYOR

    switch ( m_fatura_turu ) {

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS:
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS:
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS:
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS:
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS:
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS:
            m_ui->comboBox_fatura_turu->setEnabled( false );
            break;
        default :
            break;
    }

    //! END
     
    m_ui->lineEdit_fis_no->setText("*");

    if ( m_fatura_turu EQ ENUM_FAT_IRSALIYESIZ_ALIS_FATURASI OR
         m_fatura_turu EQ ENUM_FAT_IRSALIYESIZ_SATIS_FATURASI) {
         m_ui->label_adres->setText( "Fatura Adresi" );
    }

    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_fis_no);
    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_fatura_para_birimi_id );

    DVZ_FILL_COMBOBOX(m_ui->comboBox_fatura_para_birimi_id );

    m_ui->comboBox_fatura_para_birimi_id->setCurrentIndex( -1 );

    m_temel_para_birimi_id   = DVZ_GET_TEMEL_PARA_BIRIMI_ID();
    m_temel_para_birimi_kodu = DVZ_GET_TEMEL_PARA_BIRIMI_KODU();

    if ( E9_KULLANICI_FISIN_URETTIGI_ENT_FISLERINI_GOREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ HAYIR ) {
        m_ui->comboBox_fis_filtresi->setVisible( false );
        m_ui->label_fis_filtresi->setVisible( false );

        m_ui->comboBox_sirket->setVisible( false );
        m_ui->label_sirket->setVisible( false );
    }

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH(512);

    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_fis_filtresi );

    DISABLE_CLEAR_ON_WIDGET ( m_ui->comboBox_fatura_turu );    

    M_FATURA_DETAYLARI  = new FATURA_DETAYLARI_STRUCT;

    m_fatura_turu_string  =   FAT_GET_FATURA_TURLERI_STRING ( m_fatura_turu );

    SQL_QUERY query ( DB );

    m_ui->tableWidget_faturanin_irsaliyeleri->hideColumn        ( 0 );
    m_ui->tableWidget_faturanin_irsaliyeleri->hideColumn        ( 1 );
    m_ui->tableWidget_faturanin_irsaliyeleri->setColumnWidth    ( 2, 10 );
    m_ui->tableWidget_faturanin_irsaliyeleri->setColumnWidth    ( 3, 10 );

    m_ui->tableWidget_faturanin_irsaliyeleri->setSortingEnabled ( false );


    M_STOK_BIRIM_ADLARI                 =   new QString [URUN_BIRIMLERI_SAYISI];
    M_STOK_BIRIM_KATSAYILARI            =   new double  [URUN_BIRIMLERI_SAYISI];
    M_STOK_CAPRAZ_BIRIMLER              =   new int     [URUN_BIRIMLERI_SAYISI];
    M_STOK_BIRIM_ALIS_FIYATLARI         =   new double  [URUN_BIRIMLERI_SAYISI];
    M_STOK_BIRIM_SATIS_FIYATLARI        =   new double  [URUN_BIRIMLERI_SAYISI];

    m_ui->textEdit_irsaliye_bilgisi->setHidden  ( true );

    m_ui->lineEdit_belge_seri->setMaxLength(3);

    SUBE_UNITE_BILGILERI_STRUCT UNITE_BILG;

    SUBE_UNITE_BILGILERI_OKU( &UNITE_BILG, SUBE_GET_UNITE_ID() );

    m_fatura_belge_seri = UNITE_BILG.fatura_belge_seri;

    m_fat_iade_edilecek_gun_sayisi = 0;

    query.PREPARE_SELECT("e9_sabit_degerler" , "irs_faturalandirilacak_gun,ontanimli_kdv_orani",
                         "sabit_deger_id = :sabit_deger_id");
    query.SET_VALUE(":sabit_deger_id" , 1);

    query.SELECT();
    query.NEXT();
    m_fat_iade_edilecek_gun_sayisi = query.VALUE(0).toInt();
    m_ontanimli_kdv_orani          = query.VALUE(1).toDouble();

    m_depo_islem_yetkisi         = SUBE_GET_DEPO_ISLEM_YETKISI();

    m_ui->commaEdit_iade_edilecek_fis_no->SET_PRECISION     ( 0 );
    m_ui->commaEdit_iade_edilecek_fis_no->VIRGULSUZ_CALIS   ( true );

    m_ui->commaEdit_iskonto_yuzdesi->SET_PRECISION          ( 2 );

    m_ui->commaEdit_kur->SET_PRECISION     ( 4 );
    m_ui->commaEdit_parite->SET_PRECISION  ( 6 );
    m_ui->commaEdit_cari_para_birimi_tutari->SET_PRECISION  ( 2 );



    QRegExp rx ( "(\\*?)(\\d+)" );
    QRegExpValidator * validator = new QRegExpValidator ( rx, this );
    m_ui->lineEdit_fis_no->setValidator(validator);    

    REGISTER_SAVER_BUTTON( m_ui->button_irsaliye_secimi );
    REGISTER_SAVER_BUTTON( m_ui->button_yazdir          );
    REGISTER_SAVER_BUTTON( m_ui->button_detaylar        );
    REGISTER_SAVER_BUTTON( m_ui->pushButton_urettigi_fisler );

    REGISTER_CHANGER_BUTTON  ( m_ui->button_fatura_secimi );

    int num_of_columns = 40;

    REGISTER_TABLE_WIDGET         ( m_ui->tableWidget_fatura_satirlari, num_of_columns );

    SET_FIS_ORDER_COLUMN_NUMBER   ( ORDER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION   ( FIS_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION    ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN   ( 10 );

    SET_FIS_ORDER_DATABASE ("fat_fatura_satirlari", "order_number", "fis_satiri_id" );


    SET_TABLE_ROW_WIDGETS        ( URUN_ID_COLUMN                           , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS        ( STOK_TAKIP_SEKLI_COLUMN                  , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS        ( PARTI_ID_COLUMN                          , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS        ( IADE_EDILEN_FAT_SATIR_ID_COLUMN          , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS        ( IRSALIYE_SATIRI_SECIMI_COLUMN            , WIDGET_PUSH_BUTTON        );
    SET_TABLE_ROW_WIDGETS        ( FATURA_SATIRI_SECIMI_COLUMN              , WIDGET_PUSH_BUTTON        );
    SET_TABLE_ROW_WIDGETS        ( URUN_KODU_COLUMN                         , WIDGET_SEARCH_EDIT        );
    SET_TABLE_ROW_WIDGETS        ( URUN_ADI_COLUMN                          , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS        ( SATIRDAKI_URUN_MIKTARI_COLUMN            , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( TEMEL_BIRIM_KATSAYISI_COLUMN             , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( IRSALIYE_SATIRI_ID_COLUMN                , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS        ( FIYAT_IRSALIYEDEN_ALINDI_COLUMN          , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS        ( URUN_BIRIMI_COLUMN                       , WIDGET_COMBO_BOX_EDITABLE );
    SET_TABLE_ROW_WIDGETS        ( SERI_PARTI_NO_KODU_COLUMN                , WIDGET_PUSH_BUTTON        );
    SET_TABLE_ROW_WIDGETS        ( ISK_ONCESI_BIRIM_FIYAT_COLUMN            , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( ISK_SONRASI_BIRIM_FIYAT_COLUMN           , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( ISKONTO_TUTARI_COLUMN                    , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( ISKONTO_YUZDESI_COLUMN                   , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( SATIR_TUTARI_COLUMN                      , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( TOPLU_ISKONTO_YUZDESI_COLUMN             , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( T_IS_SONRASI_BIRIM_FIYAT_COLUMN          , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( T_IS_SONRASI_SATIR_TUTARI_COLUMN         , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN    , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( T_IS_SONRASI_OTV_TUTARI_COLUMN           , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN      , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( T_IS_SONRASI_KDV_TUTARI_COLUMN           , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN      , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( KDV_DAHIL_MI_COLUMN                      , WIDGET_COMBO_BOX          );
    SET_TABLE_ROW_WIDGETS        ( KDV_ORANI_COLUMN                         , WIDGET_COMBO_BOX          );
    SET_TABLE_ROW_WIDGETS        ( OTV_ORANI_COLUMN                         , WIDGET_COMBO_BOX          );
    SET_TABLE_ROW_WIDGETS        ( KDV_TUTARI_COLUMN                        , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( KDV_DAHIL_TUTAR_COLUMN                   , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( VERGI_HARIC_TUTAR_COLUMN                 , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( OTV_TUTARI_COLUMN                        , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( OTV_DAHIL_TUTAR_COLUMN                   , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS        ( SERI_NO_LIST_COLUMN                      , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS        ( FATURA_PARA_BIRIMI_COLUMN                , WIDGET_LINE_EDIT          );

    SET_FIRST_FOCUS_WIDGET ( m_ui->lineEdit_fis_no );

    M_FIS->acik_kapali_fatura  = E9_GET_ACIK_KAPALI_FATURA_SABIT_DEGER();

    GET_VERGI_DAIRELERI ( m_ui->comboBox_vergi_dairesi );

    REGISTER_BUTTON_SHORT_CUT ( SERI_PARTI_NO_KODU_COLUMN, QKeySequence(Qt::CTRL + Qt::Key_N ) );

    m_ui->lineEdit_irsaliye_belge_seri->setMaxLength( 3 );
    m_ui->lineEdit_irsaliye_belge_numarasi->setMaxLength  ( 15 );

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH ( 512 );
    m_ui->limitedTextEdit_adres->SET_MAX_STRING_LENGTH    ( 512 );
    m_ui->lineEdit_fis_no->setMaxLength                   ( 10   );
    m_ui->lineEdit_belge_numarasi->setMaxLength           ( 6   );
    m_ui->lineEdit_cari_hesap_ismi->setMaxLength          ( 60  );
    SET_COMBOBOX_LENGTH                                   ( m_ui->comboBox_vergi_dairesi, 50 );
    m_ui->lineEdit_vergi_no->setMaxLength                 ( 15 );

    m_ui->textEdit_irsaliye_bilgisi->setHidden              ( true );


    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {
        m_ui->button_detaylar->setHidden(true);
    }

    if ( E9_KULLANICININ_CALISTIGI_SIRKET() EQ 1 ) {
        M_FATURA_DETAYLARI->kdv_muaf_mi = 0;
        M_FATURA_DETAYLARI->otv_muaf_mi = 0;
    }
    else {
        M_FATURA_DETAYLARI->kdv_muaf_mi = 1;
        M_FATURA_DETAYLARI->otv_muaf_mi = 1;
    }

    //! 1 iade faturasi kontrolu

    if ( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
        M_FIS->iade_faturasi_mi = ENUM_FAT_IADE;
        FATURA_KDV_OTV_DEGISKENLERINE_ILK_DEGERLERINI_YAZ  ( M_FIS );
    }
    else {
        M_FIS->iade_faturasi_mi = ENUM_FAT_NORMAL;
    }

    //! 1

    //! Font
    int     point_size  = m_ui->tableWidget_fatura_satirlari->font().pointSize();
    QFont   font        = m_ui->tableWidget_fatura_satirlari->font();

    font.setPointSize ( point_size -2 );
    m_ui->tableWidget_fatura_satirlari->horizontalHeader()->setFont ( font );
    //! End Font

    FOCUS_FIRST_WIDGET      ();

    AUTOSTART_FIS_KERNEL    ( m_fis_id );
}

/**************************************************************************************
                   FAT_FATURA_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void FAT_FATURA_FISI::CLEAR_FORM_MEMBERS()
{
    m_last_urun_id                  = -1;
    m_last_fis_id                   = -1;
    m_last_urun_id                  = -1;

    m_log_kaydi_eklenecek_mi        =   false;
    m_kayit_eklendi                 =   false;
    m_kayit_silindi                 =   false;

    M_FATURA_DETAYLARI->hizmet_urt_maliyeti_hesap_id    =  0;
    M_FATURA_DETAYLARI->yurtici_satislar_hesap_id       =  0;
    M_FATURA_DETAYLARI->tevkifatli_fatura_mi            =  0;
    M_FATURA_DETAYLARI->tevkifat_yuzdesi                =  0;
    M_FATURA_DETAYLARI->satistan_iade_hesap_id          =  0;
    M_FATURA_DETAYLARI->muh_fis_no                      =  0;
    M_FATURA_DETAYLARI->cari_fis_no                     =  0;
    M_FATURA_DETAYLARI->kdv_muaf_mi                     = -1;
    M_FATURA_DETAYLARI->otv_muaf_mi                     = -1;

    FATURA_CLEAR_FIS_STRUCT         ( M_FIS );
    FATURA_CLEAR_FIS_SATIRI_STRUCT  ( M_FIS_SATIRI );
}

/**************************************************************************************
                   FAT_FATURA_FISI::NEW_FIS_RECORD
***************************************************************************************/

void FAT_FATURA_FISI::NEW_FIS_RECORD()
{

    m_ui->label_temel_para_birimi->setHidden        ( true  );
    m_ui->commaEdit_kur->setHidden                  ( true  );
    m_ui->label_cari_para_birimi->setHidden         ( true  );
    m_ui->label_fatura_para_birimi_1->setHidden     ( true  );
    m_ui->label_fatura_para_birimi_2->setHidden     ( true  );
    m_ui->commaEdit_parite->setHidden               ( true  );
    m_ui->label_parite->setHidden                   ( true  );

    M_FIS->fatura_turu          = m_fatura_turu;
    M_FIS->acik_kapali_fatura   = ENUM_ACIK_FATURA;

    FATURA_EKRANINI_DUZENLE ();

    m_ui->lineEdit_fis_no->setText("*");

    m_ui->comboBox_acik_kapali_fatura->setCurrentIndex ( m_ui->comboBox_acik_kapali_fatura->findText ( FAT_GET_ACIK_KAPALI_FATURA_STRING ( M_FIS->acik_kapali_fatura ) ) );

    m_ui->lineEdit_cari_hesap_ismi->setReadOnly( false );
    m_ui->textEdit_irsaliye_bilgisi->setHidden( true  );
    m_ui->label_parite->setVisible( false );
    m_ui->label_cari_para_birimi_tutari->setVisible( false );
    m_ui->commaEdit_kur->setVisible( false );
    m_ui->commaEdit_cari_para_birimi_tutari->setVisible( false );

    FATURA_KDV_OTV_DEGISKENLERINE_ILK_DEGERLERINI_YAZ ( M_FIS );


    M_FATURA_DETAYLARI->kdv_muaf_mi = 0;
    M_FATURA_DETAYLARI->otv_muaf_mi = 0;
}

/**************************************************************************************
                   FAT_FATURA_FISI::GET_FIS_RECORD
***************************************************************************************/

int FAT_FATURA_FISI::GET_FIS_RECORD ( int p_fis_id )
{
    SQL_QUERY s_query       ( DB );

    FATURA_FIS_BILGILERINI_OKU(M_FIS , p_fis_id );

    M_FATURA_DETAYLARI->yurtici_satislar_hesap_id    = M_FIS->yurtici_satislar_hesap_id;
    M_FATURA_DETAYLARI->hizmet_urt_maliyeti_hesap_id = M_FIS->hizmet_urt_maliyeti_hesap_id;
    M_FATURA_DETAYLARI->tevkifatli_fatura_mi         = M_FIS->tevkifatli_fatura_mi;
    M_FATURA_DETAYLARI->tevkifat_yuzdesi             = M_FIS->tevkifat_yuzdesi;
    M_FATURA_DETAYLARI->kdv_muaf_mi                  = M_FIS->kdv_muaf_mi;
    M_FATURA_DETAYLARI->otv_muaf_mi                  = M_FIS->otv_muaf_mi;
    M_FATURA_DETAYLARI->satistan_iade_hesap_id       = M_FIS->satistan_iade_hesap_id;

    int cari_fis_id   = CARI_GET_ENT_FIS_ID ( M_FIS->program_id, M_FIS->modul_id, p_fis_id );
    int muh_fis_id    = MUH_GET_ENT_FIS_ID  ( M_FIS->program_id, M_FIS->modul_id, p_fis_id );

    M_FATURA_DETAYLARI->muh_fis_no  = E9_GET_FIS_NO ( CARI_MODULU,     cari_fis_id );
    M_FATURA_DETAYLARI->cari_fis_no = E9_GET_FIS_NO ( MUHASEBE_MODULU, muh_fis_id  );

    m_fatura_turu_string         = FAT_GET_FATURA_TURLERI_STRING ( M_FIS->fatura_turu );

    m_iade_faturasi_mi           = M_FIS->iade_faturasi_mi;

    FATURA_EKRANINI_DUZENLE                     ();

    CARI_BORC_BILGISINI_GOSTER( M_FIS->cari_hesap_id );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_fatura_para_birimi_id, M_FIS->fatura_para_birimi_kodu );

    //! parite or kur
    m_ui->commaEdit_kur->SET_DOUBLE( M_FIS->doviz_kuru );
    m_ui->commaEdit_parite->SET_DOUBLE( M_FIS->parite );

    FAT_KUR_VE_PARITE_AYARLA( false );
    //! end parite or kur

    //! YENI SATIS FATURASI YAZDIRILMIS ISE DEGISIKLIK YAPILABILMESI ICIN ENABLE OLMALI

    if ( M_FIS->fatura_alis_satis_turu EQ ENUM_SATIS_IRSALIYESI_FATURASI ) {
        if ( M_FIS->yazdirildi_mi EQ 1 ) {
            m_ui->lineEdit_belge_seri->setEnabled( true );
            m_ui->lineEdit_belge_numarasi->setEnabled( true );
        }
    }

    //! END

    m_ui->lineEdit_fis_no->setText( QVariant ( M_FIS->fis_no ).toString() );
    m_ui->adakDate_fatura_tarihi->SET_DATE( M_FIS->fatura_tarihi );
    m_ui->lineEdit_belge_seri->setText( M_FIS->belge_seri );
    m_ui->lineEdit_belge_numarasi->setText( M_FIS->belge_numarasi   );


    m_ui->comboBox_acik_kapali_fatura->setCurrentIndex ( m_ui->comboBox_acik_kapali_fatura->findText ( FAT_GET_ACIK_KAPALI_FATURA_STRING ( M_FIS->acik_kapali_fatura ) ) );


    int cari_para_birimi_id = 0;

    if ( M_FIS->cari_hesap_id > 0 ) {

        QString cari_hesap_kodu, cari_hesap_ismi;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( M_FIS->cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi, &cari_para_birimi_id );


        if ( result > 0 ) {
            M_FIS->cari_hesap_ismi  = cari_hesap_ismi;
            m_ui->searchEdit_cari_hesap_kodu->SET_TEXT  ( cari_hesap_kodu );
            m_ui->lineEdit_cari_hesap_ismi->setText     ( M_FIS->cari_hesap_ismi );
            m_ui->lineEdit_cari_para_birimi->setText    ( DVZ_GET_DOVIZ_KODU ( cari_para_birimi_id ) );
            m_ui->lineEdit_cari_hesap_ismi->setReadOnly ( true );
        }
    }

    m_ui->limitedTextEdit_aciklama->setPlainText        ( M_FIS->aciklama );
    m_ui->limitedTextEdit_adres->setText                ( M_FIS->cari_hesap_adresi );
    m_ui->comboBox_vergi_dairesi->setCurrentIndex       ( m_ui->comboBox_vergi_dairesi->findText ( M_FIS->vergi_dairesi ) );
    m_ui->lineEdit_vergi_no->setText                    ( M_FIS->vergi_numarasi );
    m_ui->timeEdit_fatura_saati->setTime                ( QTime::fromString ( M_FIS->islem_saati, "hh:mm" ) );
    m_ui->commaEdit_kur->SET_DOUBLE                     ( M_FIS->doviz_kuru );
    m_ui->commaEdit_parite->SET_DOUBLE                  ( M_FIS->parite );
    m_ui->commaEdit_cari_para_birimi_tutari->SET_DOUBLE ( M_FIS->cari_para_birimi_tutari );

    STK_IRS_FIS_STRUCT * IRS_FISI = new STK_IRS_FIS_STRUCT;

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
        {
            s_query.PREPARE_SELECT("fat_irs_baglantisi",
                                   "irsaliye_id ",
                                   "fatura_id  = :fis_id");

            s_query.SET_VALUE(":fis_id", p_fis_id );

            if ( s_query.SELECT() EQ 0 ) {
                break;
            }

            s_query.NEXT();

            int irsaliye_id = s_query.VALUE(0).toInt();

            STK_IRS_CLEAR_FIS_STRUCT(IRS_FISI);

            STK_IRS_FIS_BILGILERINI_OKU(irsaliye_id , IRS_FISI);

            M_FIS->depo_id = IRS_FISI->depo_id;

            QString depo_adi, depo_kodu;

            SUBE_GET_DEPO_KODU_ADI( M_FIS->depo_id, &depo_kodu, &depo_adi );

            m_ui->searchEdit_depo_kodu->SET_TEXT    ( depo_kodu );
            m_ui->lineEdit_depo_ismi->setText       ( depo_adi  );

            m_ui->lineEdit_irsaliye_belge_seri->setText( IRS_FISI->belge_seri );
            m_ui->lineEdit_irsaliye_belge_numarasi->setText( IRS_FISI->belge_numarasi );

        }
        break;

        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS           :
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS          :

        s_query.PREPARE_SELECT("fat_irs_baglantisi" , "irsaliye_id",
                               "fatura_id = :fatura_id");

            s_query.SET_VALUE(":fatura_id", p_fis_id );

            if ( s_query.SELECT("irsaliye_id ASC") EQ 0 ) {
                break;
            }

            while ( s_query.NEXT() EQ true ) {

                m_ui->tableWidget_faturanin_irsaliyeleri->insertRow(m_ui->tableWidget_faturanin_irsaliyeleri->rowCount());

                int last_row = m_ui->tableWidget_faturanin_irsaliyeleri->rowCount() - 1;

                int irsaliye_id = s_query.VALUE(0).toInt();

                STK_IRS_CLEAR_FIS_STRUCT(IRS_FISI);

                STK_IRS_FIS_BILGILERINI_OKU(irsaliye_id , IRS_FISI);

                QTableWidgetItem * irs_id_item         =   new QTableWidgetItem ( QVariant(irsaliye_id).toString());
                QTableWidgetItem * fat_id_item         =   new QTableWidgetItem ( QVariant ( p_fis_id ).toString() );
                QTableWidgetItem * irs_seri_no_item    =   new QTableWidgetItem ( IRS_FISI->belge_seri + " " + IRS_FISI->belge_numarasi      );
                QTableWidgetItem * irs_tarihi_item     =   new QTableWidgetItem ( QDate::fromString ( IRS_FISI->fis_tarihi, "yyyy.MM.dd" ).toString("dd MM yyyy"));

                m_ui->tableWidget_faturanin_irsaliyeleri->setItem ( last_row, 0, irs_id_item      );
                m_ui->tableWidget_faturanin_irsaliyeleri->setItem ( last_row, 1, fat_id_item      );
                m_ui->tableWidget_faturanin_irsaliyeleri->setItem ( last_row, 2, irs_seri_no_item );
                m_ui->tableWidget_faturanin_irsaliyeleri->setItem ( last_row, 3, irs_tarihi_item  );

            }

            break;

        default :
            break;
    }

    FATURA_BAGLANTI_BILGISINI_GOSTER( M_FIS->fatura_turu, p_fis_id );

    m_ui->commaEdit_brut_tutar->SET_DOUBLE                      ( M_FIS->brut_tutar                  );
    m_ui->commaEdit_ara_toplam->SET_DOUBLE                      ( M_FIS->ara_toplam                  );
    m_ui->commaEdit_isk_sonrasi_kdv_tutari->SET_DOUBLE          ( M_FIS->isk_sonrasi_kdv_tutari      );
    m_ui->commaEdit_fatura_tutari->SET_DOUBLE                   ( M_FIS->fatura_tutari               );
    m_ui->commaEdit_isk_sonrasi_otv_tutari->SET_DOUBLE          ( M_FIS->isk_sonrasi_otv_tutari      );
    m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->SET_DOUBLE     ( M_FIS->isk_sonrasi_otv_dahil_tutar );
    m_ui->commaEdit_iskonto_yuzdesi->SET_DOUBLE                 ( M_FIS->iskonto_yuzdesi             );
    m_ui->commaEdit_iskonto_tutari->SET_DOUBLE                  ( M_FIS->iskonto_tutari              );


    if (  M_FIS->iptal_mi EQ 1 ) {
        m_ui->label_iptal->setVisible ( true );
        m_ui->button_yazdir->setVisible( false );
    }
    else {
        m_ui->button_yazdir->setVisible(true);
    }

    m_ui->textEdit_kdv_tutarlari->setText        ( M_FIS->kdv_oran_bilgileri);
    m_ui->textEdit_doviz_bilgileri->setText      ( M_FIS->doviz_tutar_bilgileri);
    m_ui->textEdit_otv_tutarlari->setText        ( M_FIS->otv_oran_bilgileri);


    if ( M_FIS->iade_edilen_fatura_id > 0 ) {

        s_query.PREPARE_SELECT("fat_faturalar" , "fis_no" , "fis_id = :fis_id");

        s_query.SET_VALUE      ( ":fis_id", M_FIS->iade_edilen_fatura_id );

        if ( s_query.SELECT() > 0 ) {
            s_query.NEXT();
            m_ui->commaEdit_iade_edilecek_fis_no->SET_INTEGER ( s_query.VALUE(0).toInt() );
        }
    }


    SQL_QUERY f_query(DB);

    f_query.PREPARE_SELECT("fat_fatura_satirlari" , "fis_satiri_id, urun_id, urun_adi, satirdaki_urun_miktari, urun_birimi, "
                           "isk_oncesi_urun_birim_fiyati, isk_sonrasi_urun_birim_fiyati, satir_iskonto_yuzdesi, "
                           "satir_iskonto_tutari, satir_tutari, doviz_id, satir_vergi_haric_tutar, kdv_orani, "
                           "satir_kdv_tutari, satir_kdv_dahil_tutar, kdv_dahil_mi, temel_birim_katsayisi, "
                           "otv_orani, satir_otv_tutari, satir_otv_dahil_tutar, fiyat_irsaliyeden_alindi, "
                           "toplu_iskonto_yuzdesi, t_is_son_birim_fiyati, t_is_son_satir_tutari, "
                           "t_is_son_str_vergi_haric_tutar, t_is_son_satir_otv_tutari, "
                           "t_is_son_satir_otv_dahil_tutar, t_is_son_satir_kdv_tutari, t_is_son_satir_kdv_dahil_tutar",
                           "fis_id = :fis_id" );

    f_query.SET_VALUE(":fis_id" , p_fis_id );

    if ( f_query.SELECT("order_number ASC") EQ 0 ) {
        return ADAK_OK;
    }

    while ( f_query.NEXT() EQ true ) {

        int row_number = ADD_NEW_LINE();

        QLineEdit   * lineEdit_urun_id                          = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  URUN_ID_COLUMN                        );
        QLineEdit   * lineEdit_urun_adi                         = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  URUN_ADI_COLUMN                       );
        QCommaEdit  * commaEdit_isk_oncesi_birim_fiyat          = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  ISK_ONCESI_BIRIM_FIYAT_COLUMN         );
        QCommaEdit  * commaEdit_isk_sonrasi_birim_fiyat         = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  ISK_SONRASI_BIRIM_FIYAT_COLUMN        );
        QCommaEdit  * commaEdit_iskonto_yuzdesi                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  ISKONTO_YUZDESI_COLUMN                );
        QCommaEdit  * commaEdit_iskonto_tutari                  = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  ISKONTO_TUTARI_COLUMN                 );
        QCommaEdit  * commaEdit_vergi_haric_tutar               = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  VERGI_HARIC_TUTAR_COLUMN              );
        QLineEdit   * lineEdit_takip_sekli                      = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  STOK_TAKIP_SEKLI_COLUMN               );
        QComboBox   * comboBox_urun_birimleri                   = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  URUN_BIRIMI_COLUMN                    );
        QCommaEdit  * commaEdit_satirdaki_urun_miktari          = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  SATIRDAKI_URUN_MIKTARI_COLUMN         );
        QCommaEdit  * commaEdit_satir_tutari                    = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  SATIR_TUTARI_COLUMN                   );
        QLineEdit   * lineEdit_parti_kodu_id                    = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  PARTI_ID_COLUMN                       );
        QLineEdit   * lineEdit_fiyat_irsaliyeden_alindi         = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  FIYAT_IRSALIYEDEN_ALINDI_COLUMN       );
        QLineEdit   * lineEdit_irsaliye_satiri_id               = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  IRSALIYE_SATIRI_ID_COLUMN             );
        QLineEdit   * lineEdit_para_birimi                      = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  FATURA_PARA_BIRIMI_COLUMN                    );
        QComboBox   * comboBox_kdv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  KDV_ORANI_COLUMN                      );
        QComboBox   * comboBox_kdv_dahil_mi                     = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  KDV_DAHIL_MI_COLUMN                   );
        QCommaEdit  * commaEdit_kdv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  KDV_DAHIL_TUTAR_COLUMN                );
        QCommaEdit  * commaEdit_kdv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  KDV_TUTARI_COLUMN                     );
        QComboBox   * comboBox_otv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  OTV_ORANI_COLUMN                      );
        QCommaEdit  * commaEdit_otv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  OTV_DAHIL_TUTAR_COLUMN                );
        QCommaEdit  * commaEdit_otv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  OTV_TUTARI_COLUMN                     );
        QCommaEdit  * commaEdit_temel_birim_katsayisi           = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  TEMEL_BIRIM_KATSAYISI_COLUMN          );
        QSearchEdit * searchEdit_urun_kodu                      = ( QSearchEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  URUN_KODU_COLUMN                      );
        QPushButton * pushButton_seri_parti_kodu                = ( QPushButton * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  SERI_PARTI_NO_KODU_COLUMN             );
        QLineEdit   * lineEdit_iade_edilen_fatura_satiri_id     = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  IADE_EDILEN_FAT_SATIR_ID_COLUMN       );
        QCommaEdit  * commaEdit_toplu_iskonto_yuzdesi           = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  TOPLU_ISKONTO_YUZDESI_COLUMN          );
        QCommaEdit  * commaEdit_t_is_sonrasi_birim_fiyat        = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  T_IS_SONRASI_BIRIM_FIYAT_COLUMN       );
        QCommaEdit  * commaEdit_t_is_sonrasi_satir_tutari       = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  T_IS_SONRASI_SATIR_TUTARI_COLUMN      );
        QCommaEdit  * commaEdit_t_is_sonrasi_vergi_haric_tutar  = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN );
        QCommaEdit  * commaEdit_t_is_sonrasi_otv_tutari         = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  T_IS_SONRASI_OTV_TUTARI_COLUMN        );
        QCommaEdit  * commaEdit_t_is_sonrasi_otv_dahil_tutar    = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN   );
        QCommaEdit  * commaEdit_t_is_sonrasi_kdv_tutari         = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  T_IS_SONRASI_KDV_TUTARI_COLUMN        );
        QCommaEdit  * commaEdit_t_is_sonrasi_kdv_dahil_tutar    = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN   );
        QLineEdit   * lineEdit_seri_no_list                     = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( row_number,  SERI_NO_LIST_COLUMN );

        QTableWidgetItem * item;

        int fis_satiri_id = f_query.VALUE("fis_satiri_id").toInt();

        item = new QTableWidgetItem (  QVariant ( fis_satiri_id ).toString() );
        m_ui->tableWidget_fatura_satirlari->setItem ( row_number, ROW_ID_COLUMN, item );

        item = new QTableWidgetItem( QVariant ( p_fis_id ).toString());
        m_ui->tableWidget_fatura_satirlari->setItem ( row_number, FIS_ID_COLUMN, item );

        if ( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
            s_query.PREPARE_SELECT("fat_iade_satir_baglantisi ","fat_iade_edilen_satir_id",
                                   "fat_satir_id = :fat_satir_id" );
            s_query.SET_VALUE ( ":fat_satir_id", fis_satiri_id );
            if ( s_query.SELECT() > 0 ) {
                s_query.NEXT();
                lineEdit_iade_edilen_fatura_satiri_id->setText(s_query.VALUE(0).toString());
            }
        }

        int urun_id = f_query.VALUE("urun_id").toInt();

        lineEdit_takip_sekli->setText("-1");
        int stok_takip_sekli = -1;

        lineEdit_urun_adi->setText ( f_query.VALUE("urun_adi").toString() );

        STK_FILL_URUN_BIRIMLERI_COMBOBOX ( urun_id, comboBox_urun_birimleri, M_STOK_BIRIM_ADLARI );

        STK_URUNLER_STRUCT * URUN = new STK_URUNLER_STRUCT;

        if ( urun_id > 0 ) {

             lineEdit_urun_id->setText( f_query.VALUE("urun_id").toString() );

             STK_URUNLER_TABLOSUNU_OKU(urun_id , URUN);

             searchEdit_urun_kodu->SET_TEXT(URUN->urun_kodu);
             lineEdit_urun_adi->setText(URUN->urun_adi);
             lineEdit_urun_adi->setReadOnly(true);
             stok_takip_sekli = URUN->stok_takip_sekli;
             lineEdit_takip_sekli->setText(QVariant(URUN->stok_takip_sekli).toString());

        }

        commaEdit_satirdaki_urun_miktari->SET_DOUBLE(f_query.VALUE("satirdaki_urun_miktari").toDouble());
        if ( urun_id > 0 ) {
            comboBox_urun_birimleri->setCurrentIndex(comboBox_urun_birimleri->findText(f_query.VALUE("urun_birimi").toString()));
        }
        else {
            comboBox_urun_birimleri->setEditText(f_query.VALUE("urun_birimi").toString());
        }

        double isk_sonrasi_birim_fiyati = f_query.VALUE("isk_sonrasi_urun_birim_fiyati").toDouble();

        commaEdit_isk_oncesi_birim_fiyat->SET_DOUBLE            ( f_query.VALUE ( "isk_oncesi_urun_birim_fiyati"  ).toDouble() );
        commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE           ( f_query.VALUE ( "isk_sonrasi_urun_birim_fiyati" ).toDouble() );
        commaEdit_iskonto_yuzdesi->SET_DOUBLE                   ( f_query.VALUE ( "satir_iskonto_yuzdesi"         ).toDouble() );
        commaEdit_iskonto_tutari->SET_DOUBLE                    ( f_query.VALUE ( "satir_iskonto_tutari"          ).toDouble() );
        commaEdit_satir_tutari->SET_DOUBLE                      ( f_query.VALUE ( "satir_tutari"                  ).toDouble() );
        lineEdit_para_birimi->setText                           ( DVZ_GET_DOVIZ_KODU ( f_query.VALUE("doviz_id").toInt() ) );
        comboBox_kdv_orani->setCurrentIndex                     ( comboBox_kdv_orani->findText(f_query.VALUE("kdv_orani").toString()));
        commaEdit_kdv_tutari->SET_DOUBLE                        ( f_query.VALUE ( "satir_kdv_tutari"        ).toDouble() );
        commaEdit_kdv_dahil_tutar->SET_DOUBLE                   ( f_query.VALUE ( "satir_kdv_dahil_tutar"   ).toDouble() );
        comboBox_kdv_dahil_mi->setCurrentIndex                  ( f_query.VALUE ( "kdv_dahil_mi"            ).toInt() );
        commaEdit_temel_birim_katsayisi->SET_DOUBLE             ( f_query.VALUE ( "temel_birim_katsayisi"   ).toDouble() );
        comboBox_otv_orani->setCurrentIndex                     ( comboBox_otv_orani->findText(f_query.VALUE("otv_orani").toString()));
        commaEdit_otv_tutari->SET_DOUBLE                        ( f_query.VALUE ( "satir_otv_tutari"         ).toDouble() );
        commaEdit_otv_dahil_tutar->SET_DOUBLE                   ( f_query.VALUE ( "satir_otv_dahil_tutar"    ).toDouble() );
        lineEdit_fiyat_irsaliyeden_alindi->setText              ( f_query.VALUE ( "fiyat_irsaliyeden_alindi" ).toString() );
        commaEdit_vergi_haric_tutar->SET_DOUBLE                 ( f_query.VALUE ( "satir_vergi_haric_tutar"  ).toDouble());

        commaEdit_toplu_iskonto_yuzdesi->SET_DOUBLE             ( f_query.VALUE ( "toplu_iskonto_yuzdesi"            ).toDouble() );
        commaEdit_t_is_sonrasi_birim_fiyat->SET_DOUBLE          ( f_query.VALUE ( "t_is_son_birim_fiyati"            ).toDouble() );
        commaEdit_t_is_sonrasi_satir_tutari->SET_DOUBLE         ( f_query.VALUE ( "t_is_son_satir_tutari"            ).toDouble() );
        commaEdit_t_is_sonrasi_vergi_haric_tutar->SET_DOUBLE    ( f_query.VALUE ( "t_is_son_str_vergi_haric_tutar" ).toDouble() );
        commaEdit_t_is_sonrasi_otv_tutari->SET_DOUBLE           ( f_query.VALUE ( "t_is_son_satir_otv_tutari"        ).toDouble() );
        commaEdit_t_is_sonrasi_otv_dahil_tutar->SET_DOUBLE      ( f_query.VALUE ( "t_is_son_satir_otv_dahil_tutar"   ).toDouble() );
        commaEdit_t_is_sonrasi_kdv_tutari->SET_DOUBLE           ( f_query.VALUE ( "t_is_son_satir_kdv_tutari"        ).toDouble() );
        commaEdit_t_is_sonrasi_kdv_dahil_tutar->SET_DOUBLE      ( f_query.VALUE ( "t_is_son_satir_kdv_dahil_tutar"   ).toDouble() );


        int irsaliye_satiri_id = -1;

        switch ( M_FIS->fatura_turu ) {

            case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS                 :
            case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS                  :
            case ENUM_FAT_IRSALIYELI_ALIS_FATURASI                   :
            case ENUM_FAT_IRSALIYELI_SATIS_FATURASI                  :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS          :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS         :
            case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS           :
            case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS          :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS  :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS :
            {
                s_query.PREPARE_SELECT ( "fat_irs_satir_baglantisi ","irsaliye_satiri_id",
                                         "fatura_satiri_id = :fatura_satiri_id");

                s_query.SET_VALUE(":fatura_satiri_id", fis_satiri_id );

                if ( s_query.SELECT() EQ 0 ) {
                    break;
                }

                s_query.NEXT();

                irsaliye_satiri_id = s_query.VALUE(0).toInt();

                lineEdit_irsaliye_satiri_id->setText ( QVariant ( irsaliye_satiri_id ).toString() );

                if ( irsaliye_satiri_id > 0 ) {

                    STK_IRS_FIS_SATIR_STRUCT * IRS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

                    STK_IRS_CLEAR_SATIR_STRUCT ( IRS_SATIRI );

                    int result = STK_IRS_SATIR_BILGILERINI_OKU ( irsaliye_satiri_id , IRS_SATIRI);

                    if ( result > 0 ) {

                        int parti_kodu_id      = IRS_SATIRI->parti_id;

                        commaEdit_satirdaki_urun_miktari->setReadOnly   ( false );
                        comboBox_urun_birimleri->setEnabled             ( true  );

                        switch ( stok_takip_sekli ) {

                            case ENUM_PARTI_TAKIBI :

                                if ( parti_kodu_id > 0 ) {
                                    lineEdit_parti_kodu_id->setText(QVariant(parti_kodu_id).toString());
                                    lineEdit_seri_no_list->setText( STK_GET_PARTI_KODU( parti_kodu_id ) );
                                }
                                break;

                            case ENUM_SERI_NO_ILE_TAKIP : {

                                // SERI NOLAR her satirin ayri seri nosu vardir search edite tiklandiktan sonra
                                // burari parse edilir. ve batch gonderilir.

                                QString seri_numaralari = STK_GET_SERI_NO_BILGILERI( irsaliye_satiri_id , ENUM_URUN_HER_IKISI );

                                lineEdit_seri_no_list->setText( seri_numaralari );

                                break;
                                }
                            case ENUM_BASIT_TAKIP       :
                            default                     :
                                pushButton_seri_parti_kodu->setEnabled( false);
                                break;
                        }
                    }
                }
                break;
            }

            default :
                break;

        }

        FATURA_SATIR_DOVIZ_TUTARLARINI_GUNCELLE ( lineEdit_para_birimi->text(), commaEdit_satir_tutari->GET_DOUBLE() ,DOVIZ_EKLE, M_FIS->DOVIZ_CINSINDEN_SATIR_TOPLAMLARI );

        switch ( M_FIS->fatura_turu ) {

            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS          :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS         :
            case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS           :
            case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS          :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS  :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS :

                if ( isk_sonrasi_birim_fiyati EQ 0 ) {
                    commaEdit_isk_sonrasi_birim_fiyat->setDisabled   ( false );
                    commaEdit_isk_oncesi_birim_fiyat->setDisabled    ( false );
                    commaEdit_iskonto_tutari->setDisabled            ( false );
                    commaEdit_iskonto_yuzdesi->setDisabled           ( false );
                    commaEdit_satir_tutari->setDisabled              ( false );
                    comboBox_kdv_orani->setDisabled                  ( false );
                    comboBox_otv_orani->setDisabled                  ( false );
                    comboBox_kdv_dahil_mi->setDisabled               ( false );
                }
                else {
                    commaEdit_isk_sonrasi_birim_fiyat->setDisabled  ( true  );
                    // commaEdit_isk_oncesi_birim_fiyat->setDisabled   ( true  );
                    comboBox_kdv_orani->setDisabled                 ( true  );
                    comboBox_otv_orani->setDisabled                 ( true  );
                    comboBox_kdv_dahil_mi->setDisabled              ( true  );

                }

                // otomatik islemlerde adet uzerinde islem  yapilamz sadece kismi israliyelestirmede yapilabilir.
                if ( M_FIS->fatura_turu EQ ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS OR
                     M_FIS->fatura_turu EQ ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS ) {
                    m_ui->button_irsaliye_secimi->setEnabled( false );
                    commaEdit_satirdaki_urun_miktari->setEnabled( false );
                }

                searchEdit_urun_kodu->setDisabled        ( true );
                comboBox_urun_birimleri->setDisabled     ( true );
                lineEdit_para_birimi->setDisabled        ( true );
                break;

            default :
                if ( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
                    commaEdit_isk_sonrasi_birim_fiyat->setDisabled  ( true  );
                    //commaEdit_isk_oncesi_birim_fiyat->setDisabled   ( true  );
                    commaEdit_iskonto_tutari->setDisabled           ( true  );
                    //commaEdit_iskonto_yuzdesi->setDisabled          ( true  );
                    //commaEdit_satir_tutari->setDisabled             ( true );
                    comboBox_kdv_orani->setDisabled                 ( true );
                    comboBox_otv_orani->setDisabled                 ( true );
                    comboBox_kdv_dahil_mi->setDisabled              ( true );
                    searchEdit_urun_kodu->setDisabled               ( true );
                    comboBox_urun_birimleri->setDisabled            ( true );
                    pushButton_seri_parti_kodu->setDisabled         ( true );
                    lineEdit_para_birimi->setDisabled               ( true );
                }
                break;
        }


        //! IADE FATURALARINDA SADECE TEK FATURAYA REFERANS EDER
        //! BUNDAN DOLAYI SATIR OLAN BIR IADE FAT. SINDA BIR DAHA FATURA
        //! SECILMESI YANLIS OLUR.
        if ( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
            m_ui->button_fatura_secimi->setEnabled( false );
        }

    }   // END OF WHILE

    m_ui->button_irsaliye_secimi->setEnabled( false );

    SATIR_DOVIZ_TUTARLARINI_YAZDIR  ();
    KDV_ORANLARI_BILGILERINI_YAZDIR ();
    OTV_ORANLARI_BILGILERINI_YAZDIR ();

    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void FAT_FATURA_FISI::SET_LINE_DEFAULTS ( int p_row_number )
{
    QSearchEdit * search_edit_urun_kodu                 = ( QSearchEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_KODU_COLUMN                     );
    QCommaEdit  * commaedit_satirdaki_urun_miktari      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIRDAKI_URUN_MIKTARI_COLUMN        );
    QCommaEdit  * commaedit_temel_birim_katsayisi       = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, TEMEL_BIRIM_KATSAYISI_COLUMN         );
    QCommaEdit  * commaedit_vergi_haric_tutar           = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTAR_COLUMN             );
    QComboBox   * combobox_kdv_dahil_mi                 = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN                  );
    QComboBox   * combobox_kdv_orani                    = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_ORANI_COLUMN                     );
    QComboBox   * comboBox_otv_orani                    = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_ORANI_COLUMN                     );
    QPushButton * button_irsaliye_satiri_secimi         = ( QPushButton * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, IRSALIYE_SATIRI_SECIMI_COLUMN        );
    QPushButton * button_fatura_satiri_secimi           = ( QPushButton * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, FATURA_SATIRI_SECIMI_COLUMN          );
    QPushButton * button_seri_parti_kodu                = ( QPushButton * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SERI_PARTI_NO_KODU_COLUMN            );
    QCommaEdit  * commaEdit_satir_tutari                = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN                  );
    QCommaEdit  * commaEdit_iskonto_oncesi_birim_fiyat  = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN        );
    QCommaEdit  * commaEdit_iskonto_sonrasi_birim_fiyat = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN       );
    QLineEdit   * lineEdit_urun_adi                     = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ADI_COLUMN                      );
    QLineEdit   * lineEdit_makbuz_para_birimi           = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, FATURA_PARA_BIRIMI_COLUMN            );
    QComboBox   * combobox_urun_birimi                  = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN                   );

    if ( m_fatura_turu EQ ENUM_FAT_HIZMET_ALIS_FATURASI OR
         m_fatura_turu EQ ENUM_FAT_HIZMET_SATIS_FATURASI ) {
        combobox_urun_birimi->setDisabled(true);
    }

    lineEdit_makbuz_para_birimi->setMaxLength( 5 );

    lineEdit_makbuz_para_birimi->setEnabled( false );

    lineEdit_makbuz_para_birimi->setText( DVZ_GET_DOVIZ_KODU( M_FIS->fatura_para_birimi_id ) );

    combobox_kdv_dahil_mi->addItems ( QStringList() << tr("Hariç") << tr("Dahil"));
    lineEdit_urun_adi->setMaxLength ( 50 );

    KDV_ORANLARI_COMBOBOX_DOLDUR ( combobox_kdv_orani );
    OTV_ORANLARI_COMBOBOX_DOLDUR ( comboBox_otv_orani );

    commaEdit_satir_tutari->SET_RETURN_EMPTY ( false );

    commaedit_temel_birim_katsayisi->SET_DOUBLE ( 1 );
    commaedit_vergi_haric_tutar->setEnabled( false );

    button_fatura_satiri_secimi->setIcon( QPixmap( ":/e9_icons/E9_ICONS/fatura.png" ));
    button_irsaliye_satiri_secimi->setIcon( QPixmap( ":/e9_icons/E9_ICONS/irsaliye.png" ));

    button_seri_parti_kodu->setIconSize( QSize( 80, 20 ));
    button_seri_parti_kodu->setIcon( QPixmap( ":/e9_icons/E9_ICONS/stok_seri_numaralari.png" ));

    button_irsaliye_satiri_secimi->setToolTip ( tr ( "İrsaliye satırı seçmek için tıklayın"         ) );
    button_fatura_satiri_secimi->setToolTip   ( tr ( "Fatura satırı seçmek için tıklayın"           ) );

    REGISTER_LINE_CHANGER_BUTTON ( p_row_number, IRSALIYE_SATIRI_SECIMI_COLUMN   );
    REGISTER_LINE_CHANGER_BUTTON ( p_row_number, FATURA_SATIRI_SECIMI_COLUMN     );
    REGISTER_LINE_CHANGER_BUTTON ( p_row_number, SERI_PARTI_NO_KODU_COLUMN     );

    combobox_kdv_orani->setCurrentIndex(combobox_kdv_orani->findText(QVariant(m_ontanimli_kdv_orani).toString()));
    if ( M_FATURA_DETAYLARI->kdv_muaf_mi EQ 1 ) {
        combobox_kdv_orani->setCurrentIndex(combobox_kdv_orani->findText("0"));
        combobox_kdv_orani->setDisabled(true);

    }
    else {
        combobox_kdv_orani->setEnabled(true);
    }

    if ( M_FATURA_DETAYLARI->otv_muaf_mi EQ 1 ) {
        comboBox_otv_orani->setCurrentIndex(comboBox_otv_orani->findText("0"));
        comboBox_otv_orani->setDisabled(true);
    }
    else {
        comboBox_otv_orani->setEnabled(true);
    }

    int fiyat_hassasiyeti = STK_GET_BIRIM_HASSASIYETI();

    commaEdit_iskonto_oncesi_birim_fiyat->SET_PRECISION( fiyat_hassasiyeti );
    commaEdit_iskonto_sonrasi_birim_fiyat->SET_PRECISION( fiyat_hassasiyeti );

    int birim_miktar_hassasiyeti = STK_GET_MIKTAR_HASSASIYETI();

    commaedit_satirdaki_urun_miktari->SET_PRECISION( birim_miktar_hassasiyeti );


    if ( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
        search_edit_urun_kodu->setEnabled( false );
        lineEdit_urun_adi->setEnabled    ( false );
        if ( M_FIS->fatura_turu EQ ENUM_FAT_VADE_FARKI_SATIS_FATURASI OR M_FIS->fatura_turu EQ ENUM_FAT_VADE_FARKI_ALIS_FATURASI ) {
            lineEdit_urun_adi->setEnabled( true );
        }
        if ( M_FIS->fatura_turu EQ ENUM_FAT_HIZMET_SATIS_FATURASI OR M_FIS->fatura_turu EQ ENUM_FAT_HIZMET_ALIS_FATURASI ) {
            search_edit_urun_kodu->setEnabled( true );
            lineEdit_urun_adi->setEnabled    ( true );
            lineEdit_urun_adi->setEnabled    ( true );

        }
    }

}

/**************************************************************************************
                   FAT_FATURA_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int FAT_FATURA_FISI::CHECK_FIS_FORM_VAR ( QObject * p_object )
{ 
    if ( p_object EQ m_ui->searchEdit_cari_hesap_kodu ) {
        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_cari_hesap_ismi->clear();
            m_ui->limitedTextEdit_adres->clear();
            m_ui->comboBox_vergi_dairesi->setCurrentIndex(-1);
            m_ui->lineEdit_vergi_no->clear();
            m_ui->lineEdit_cari_para_birimi->clear();
            m_ui->comboBox_vergi_dairesi->setEnabled(true);
            m_ui->lineEdit_vergi_no->setEnabled(true);
            M_FIS->cari_hesap_id       = -1;
            return ADAK_OK;
        }

        int cari_hesap_id, para_birim_id;
        QString cari_hesap_ismi;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT(), &cari_hesap_id, &cari_hesap_ismi, &para_birim_id );
        if ( result EQ 0 ) {
            MSG_WARNING(  tr("Aradığınız cari hesap kodu bulunamadı!.."), m_ui->searchEdit_cari_hesap_kodu );
            return ADAK_FAIL_UNDO;
        }

        m_ui->lineEdit_cari_hesap_ismi->setReadOnly(true);

        M_FIS->cari_hesap_id       = cari_hesap_id;
        M_FIS->cari_para_birimi_id = para_birim_id;
        M_FIS->fatura_para_birimi_id = para_birim_id; // ilk basta cari ile makbuz para birimi ayni
        M_FIS->fatura_para_birimi_kodu = DVZ_GET_DOVIZ_KODU( para_birim_id );

        FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_fatura_para_birimi_id, M_FIS->fatura_para_birimi_kodu );

        m_ui->commaEdit_kur->SET_DOUBLE ( DVZ_DOVIZ_KURUNU_AL( M_FIS->cari_para_birimi_id, m_ui->adakDate_fatura_tarihi->QDATE(), ENUM_SATIS ) );

        m_ui->lineEdit_cari_hesap_ismi->setText  ( cari_hesap_ismi );
        m_ui->lineEdit_cari_para_birimi->setText ( DVZ_GET_DOVIZ_KODU ( M_FIS->cari_para_birimi_id ) );

        m_ui->limitedTextEdit_adres->setText( CARI_GET_ADRES_BILGILERI( cari_hesap_id ) );

        QString vergi_no, vergi_dairesi;

        result = CARI_GET_VERGI_NO_VERGI_DAIRESI( cari_hesap_id, &vergi_no, &vergi_dairesi );

        if ( result EQ 1 ) {
            m_ui->comboBox_vergi_dairesi->setEditText(vergi_dairesi);
            m_ui->lineEdit_vergi_no->setText              ( vergi_no );

        }

        if ( m_ui->comboBox_vergi_dairesi->currentIndex() NE -1 AND
             m_ui->lineEdit_vergi_no->text().isEmpty() EQ false ) {
            m_ui->comboBox_vergi_dairesi->setDisabled(true);
            m_ui->lineEdit_vergi_no->setDisabled(true);

        }
        else {
            m_ui->comboBox_vergi_dairesi->setEnabled(true);
            m_ui->lineEdit_vergi_no->setEnabled(true);;
        }

        CARI_BORC_BILGISINI_GOSTER( cari_hesap_id );

        if( M_FIS->cari_para_birimi_id NE m_temel_para_birimi_id ) {

            m_ui->commaEdit_kur->SET_DOUBLE( DVZ_PARA_BIRIMLERINI_DONUSTUR ( M_FIS->cari_para_birimi_id, 1.00, m_temel_para_birimi_id, m_ui->adakDate_fatura_tarihi->QDATE(), ENUM_SATIS ) );

            m_ui->label_fatura_para_birimi_1->setVisible( true );
            m_ui->label_fatura_para_birimi_1->setText( "Kur  : 1 "+ M_FIS->fatura_para_birimi_kodu );
            m_ui->commaEdit_kur->setVisible( true );
            m_ui->label_temel_para_birimi->setVisible( true );
        }

        SATIR_PARA_BIRIMINI_GUNCELLE();
    }
    else if ( p_object EQ m_ui->searchEdit_depo_kodu ) {

        if ( m_ui->searchEdit_depo_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->searchEdit_depo_kodu->SET_TEXT("");
            m_ui->lineEdit_depo_ismi->clear();
            M_FIS->depo_id = -1;
            return ADAK_OK;
        }

        QString depo_adi;
        int     depo_id;
        int result = SUBE_GET_DEPO_ID_ADI( m_ui->searchEdit_depo_kodu->GET_TEXT(), &depo_id, &depo_adi );

        if ( result EQ 0 ) {
            MSG_WARNING( tr("Aradığınız depo kodu bulunamadı!.."), m_ui->searchEdit_depo_kodu );

            return ADAK_FAIL_UNDO;
        }
        M_FIS->depo_id = depo_id;
        m_ui->lineEdit_depo_ismi->setText( depo_adi );
    }
    else if ( p_object EQ m_ui->tableWidget_faturanin_irsaliyeleri ) {
        if ( p_object->objectName() EQ "ITEM_DOUBLE_CLICKED" ) {
            int current_row = m_ui->tableWidget_faturanin_irsaliyeleri->currentRow();
            SECILI_IRSALIYENIN_SATIRLARINI_FATURADAN_SIL ( current_row );
        }
    }
    else if ( p_object EQ m_ui->lineEdit_belge_numarasi OR p_object EQ m_ui->lineEdit_belge_seri ) {
        m_ui->lineEdit_belge_seri->setText ( m_ui->lineEdit_belge_seri->text().toUpper() );
    }
    else if ( p_object EQ m_ui->lineEdit_irsaliye_belge_numarasi OR p_object EQ m_ui->lineEdit_irsaliye_belge_seri ) {
        m_ui->lineEdit_irsaliye_belge_seri->setText( m_ui->lineEdit_irsaliye_belge_seri->text().toUpper() );
    }
    else if ( p_object EQ m_ui->commaEdit_iskonto_tutari OR p_object EQ m_ui->commaEdit_iskonto_yuzdesi ) {

        if ( p_object EQ m_ui->commaEdit_iskonto_tutari ) {

            if ( m_ui->commaEdit_iskonto_tutari->GET_DOUBLE() > M_FIS->brut_tutar ) {
               MSG_WARNING( tr("Fatura vergi hariç toplamından daha büyük iskonto uygulayamazsınız."), m_ui->commaEdit_iskonto_tutari );

                return ADAK_FAIL_UNDO;
            }
            M_FIS->iskonto_tutari  = m_ui->commaEdit_iskonto_tutari->GET_DOUBLE();
            // hassasiyetin artirilmasinin sebebi tutardan-> yuzde gecerken kayip olmasi, boylece kayip olmuyor.
            M_FIS->iskonto_yuzdesi = ROUND ( ( M_FIS->iskonto_tutari / M_FIS->brut_tutar ) * 100.00, -4 );
            m_ui->commaEdit_iskonto_yuzdesi->SET_DOUBLE ( M_FIS->iskonto_yuzdesi );
        }
        else {
            if ( m_ui->commaEdit_iskonto_yuzdesi->GET_DOUBLE() > 100.00 ) {
                MSG_WARNING( tr("İskonto %100 den daha büyük olamaz."), m_ui->commaEdit_iskonto_yuzdesi );

                return ADAK_FAIL_UNDO;
            }
            M_FIS->iskonto_yuzdesi  = m_ui->commaEdit_iskonto_yuzdesi->GET_DOUBLE();
            M_FIS->iskonto_tutari   = ROUND ( ( M_FIS->iskonto_yuzdesi * M_FIS->brut_tutar ) / 100.00 );
            m_ui->commaEdit_iskonto_tutari->SET_DOUBLE ( M_FIS->iskonto_tutari );
        }

        FATURA_ISKONTO_ORANINA_GORE_TOPLAMLARI_HESAPLA ( M_FIS );
        FAT_FIS_TOPLAMLARINI_YENILE( M_FIS );

        CLOSE_ALL_EVENTS    ();
        for ( int i = 0; i < m_ui->tableWidget_fatura_satirlari->rowCount() -1; i++ ) {
            TOPLU_ISKONTO_SONRASI_SATIR_DEGERLERINI_HESAPLA ( i );
        }
        OPEN_ALL_EVENTS ();
    }
    else if ( p_object EQ m_ui->commaEdit_kur ) {
        m_ui->commaEdit_cari_para_birimi_tutari->SET_DOUBLE ( m_ui->commaEdit_kur->GET_DOUBLE() * m_ui->commaEdit_fatura_tutari->GET_DOUBLE() );
    }
    else if ( p_object EQ m_ui->comboBox_fatura_turu ) {

        QString fatura_turu = m_ui->comboBox_fatura_turu->currentText();

        switch ( M_FIS->fatura_alis_satis_turu ) {
            case ENUM_ALIS_IRSALIYESI_FATURASI :
            default                                     :
                m_fatura_turu_string = fatura_turu + tr(" ( ALIŞ )");
                break;
            case ENUM_SATIS_IRSALIYESI_FATURASI :
                m_fatura_turu_string = fatura_turu + tr(" ( SATIŞ )");
                break;
        }

        m_ui->comboBox_fatura_turu->setCurrentIndex ( m_ui->comboBox_fatura_turu->findText (  m_fatura_turu_string ) );
        m_fatura_turu      = FAT_GET_FATURA_TURLERI_ENUM ( m_fatura_turu_string );
        M_FIS->fatura_turu = m_fatura_turu;
        return ADAK_NEW_RECORD;
    }
    else if( p_object EQ m_ui->comboBox_fatura_para_birimi_id ) {

        //! KONTROL
        SQL_QUERY sql_query ( DB );
        int fis_id = GET_RECORD_ID();

        if ( fis_id NE -1 ) {
            sql_query.PREPARE_SELECT ( "fat_fatura_satirlari","fis_satiri_id",
                                       "fis_id = :fis_id" );

            sql_query.SET_VALUE      ( ":fis_id", fis_id );

            if ( sql_query.SELECT() NE 0 ) {
                MSG_WARNING( tr ("Fiş satırlarında işlem bulunmaktadır.Makbuz para birimini değiştiremezsiniz."), m_ui->comboBox_fatura_para_birimi_id );
                return ADAK_FAIL_UNDO;
            }
        }

        if( M_FIS->cari_hesap_id < 1 ) {
            MSG_WARNING( "Önce Cari Hesap Seçmelisiniz.!", m_ui->searchEdit_cari_hesap_kodu );
            return ADAK_FAIL;
        }
        //! END KONTROL

        M_FIS->fatura_para_birimi_id = DVZ_GET_DOVIZ_ID( m_ui->comboBox_fatura_para_birimi_id->currentText() );
        M_FIS->fatura_para_birimi_kodu = m_ui->comboBox_fatura_para_birimi_id->currentText();


        FAT_KUR_VE_PARITE_AYARLA( true );

        SATIR_PARA_BIRIMINI_GUNCELLE();

        if( M_FIS->fatura_para_birimi_id NE M_FIS->cari_para_birimi_id ) {
            m_ui->label_cari_para_birimi_tutari->setVisible( true );
            m_ui->commaEdit_cari_para_birimi_tutari->setVisible( true );
        }
        else {
            m_ui->label_cari_para_birimi_tutari->setVisible( false );
            m_ui->commaEdit_cari_para_birimi_tutari->setVisible( false );
        }
    }

    return ADAK_OK;    
}

/**************************************************************************************
                   FAT_FATURA_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int FAT_FATURA_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( m_ui->lineEdit_fis_no->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Yeni Fatura Kaydı Yapabilmek için, Fatura No değeri olarak (*) girmelisiniz!.."), m_ui->lineEdit_fis_no );

        return ADAK_FAIL;
    }
    switch ( M_FIS->fatura_turu ) {
        case ENUM_FAT_IRSALIYESIZ_ALIS_FATURASI                 :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI                  :
        case ENUM_FAT_HIZMET_ALIS_FATURASI                      :
        case ENUM_FAT_VADE_FARKI_ALIS_FATURASI                  :
        case ENUM_FAT_HAKEDIS_ALIS_FATURASI                     :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS         :
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS          :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS                 :
            if ( m_iade_faturasi_mi NE 1 ) {
                if ( m_ui->lineEdit_belge_seri->text().isEmpty() EQ true ) {
                    MSG_INFO(QObject::tr("Alış Faturalarında Fatura Seri / No bilgisini boş bırakamazsınız."),m_ui->lineEdit_belge_seri);
                    return ADAK_FAIL;
                }
                if ( m_ui->lineEdit_belge_numarasi->text().isEmpty() EQ true ) {
                    MSG_INFO(QObject::tr("Alış Faturalarında Fatura Seri / No bilgisini boş bırakamazsınız."),m_ui->lineEdit_belge_numarasi);
                    return ADAK_FAIL;
                }
            }
            break;

        case ENUM_FAT_IRSALIYESIZ_SATIS_FATURASI                 :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI                  :
        case ENUM_FAT_HIZMET_SATIS_FATURASI                      :
        case ENUM_FAT_VADE_FARKI_SATIS_FATURASI                  :
        case ENUM_FAT_HAKEDIS_SATIS_FATURASI                     :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS         :
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS          :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS                 :
            if ( m_iade_faturasi_mi EQ 1 ) {
                if ( m_ui->lineEdit_belge_seri->text().isEmpty() EQ true ) {
                    MSG_INFO(QObject::tr("Alış Faturalarında Fatura Seri / No bilgisini boş bırakamazsınız."),m_ui->lineEdit_belge_seri);
                    return ADAK_FAIL;
                }
                if ( m_ui->lineEdit_belge_numarasi->text().isEmpty() EQ true ) {
                    MSG_INFO(QObject::tr("Alış Faturalarında Fatura Seri / No bilgisini boş bırakamazsınız."),m_ui->lineEdit_belge_numarasi);
                    return ADAK_FAIL;
                }
            }
        break;
        default:
        break;
    }
    if ( M_FIS->cari_hesap_id < 1 ) {
        MSG_WARNING( tr("Cari hesap seçimi yapılmamaış."), m_ui->searchEdit_cari_hesap_kodu );

        return ADAK_FAIL;
    }

    switch ( M_FIS->fatura_turu ) {
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :

            if ( M_FIS->depo_id < 1 ) {
                MSG_WARNING( tr("Depo seçimi yapılmamış!.."), m_ui->searchEdit_depo_kodu );

                return ADAK_FAIL;
            }
            break;

        default :
            break;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int FAT_FATURA_FISI::CHECK_FIS_RECORD_ADD()
{
    SQL_QUERY query ( DB );

    if ( QString ( m_ui->lineEdit_fis_no->text().at(0)) NE "*") {

        query.PREPARE_SELECT ( "fat_faturalar ","fis_id",
                               "fis_no        = :fis_no "
                               "AND fatura_tarihi   = :fatura_tarihi");

        query.SET_VALUE(":fis_no"          , m_ui->lineEdit_fis_no->text().toInt());
        query.SET_VALUE(":fatura_tarihi"   , m_ui->adakDate_fatura_tarihi->DATE());

        if ( query.SELECT() NE 0 ) {
            MSG_WARNING( tr("Aynı fiş numarasıyla kayıtlı , başka bir fatura bulunmaktadır.Lütfen fiş numarasını değiştiriniz!.."), m_ui->lineEdit_fis_no );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() NE true ) {

        if( BELGE_SERI_KONTROLU( m_ui->lineEdit_belge_seri->text(), m_ui->lineEdit_belge_numarasi->text(), FATURA_MODULU, M_FIS->cari_hesap_id ) EQ false ) {
            int secim = MSG_YES_NO( "Ayni Belge Seri + Belge Numarasi kayıtlı!. Devam Etmek istermisiniz ?", m_ui->lineEdit_belge_seri );
            if( secim EQ ADAK_NO ) {
                return ADAK_FAIL;
            }
        }
        if( BELGE_SERI_KONTROLU( m_ui->lineEdit_irsaliye_belge_seri->text(), m_ui->lineEdit_irsaliye_belge_numarasi->text(), IRSALIYE_MODULU, M_FIS->cari_hesap_id ) EQ false ) {
            int secim = MSG_YES_NO( "Ayni Belge Seri + Belge Numarasi kayıtlı!. Devam Etmek istermisiniz ?", m_ui->lineEdit_irsaliye_belge_seri );
            if( secim EQ ADAK_NO ) {
                return ADAK_FAIL;
            }
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_FISI::ADD_FIS_RECORD
***************************************************************************************/

int FAT_FATURA_FISI::ADD_FIS_RECORD()
{
    if ( QString ( m_ui->lineEdit_fis_no->text().at(0) ) NE "*" ) {
        M_FIS->fis_no = m_ui->lineEdit_fis_no->text().toInt();
    }

    M_FIS->acik_kapali_fatura = FAT_GET_ACIK_KAPALI_FATURA_ENUM ( m_ui->comboBox_acik_kapali_fatura->currentText() );

    if ( ( M_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_ALIS_FATURASI  OR  M_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) OR
         ( M_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS  OR  M_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS ) ) {
        M_FIS->fatura_irsaliyelestirildi_mi = 1;
    }

    M_FIS->fatura_tarihi                    = m_ui->adakDate_fatura_tarihi->DATE();
    M_FIS->belge_seri                       = m_ui->lineEdit_belge_seri->text();
    M_FIS->belge_numarasi                   = m_ui->lineEdit_belge_numarasi->text();
    M_FIS->cari_hesap_ismi                  = m_ui->lineEdit_cari_hesap_ismi->text();
    M_FIS->aciklama                         = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->cari_hesap_adresi                = m_ui->limitedTextEdit_adres->toPlainText();
    M_FIS->vergi_dairesi                    = m_ui->comboBox_vergi_dairesi->currentText();
    M_FIS->vergi_numarasi                   = m_ui->lineEdit_vergi_no->text();
    M_FIS->islem_saati                      = m_ui->timeEdit_fatura_saati->time().toString ( "hh:mm" );
    M_FIS->iade_faturasi_mi                 = m_iade_faturasi_mi;
    M_FIS->iskonto_yuzdesi                  = m_ui->commaEdit_iskonto_yuzdesi->GET_DOUBLE();
    M_FIS->iskonto_tutari                   = m_ui->commaEdit_iskonto_tutari->GET_DOUBLE();
    M_FIS->fatura_para_birimi_id            = DVZ_GET_DOVIZ_ID( m_ui->comboBox_fatura_para_birimi_id->currentText() );
    M_FIS->doviz_kuru                       = m_ui->commaEdit_kur->GET_DOUBLE();
    M_FIS->parite                           = m_ui->commaEdit_parite->GET_DOUBLE();
    M_FIS->cari_para_birimi_tutari          = m_ui->commaEdit_cari_para_birimi_tutari->GET_DOUBLE();
    M_FIS->modul_id                         = FATURA_MODULU;
    M_FIS->program_id                       = E9_PROGRAMI;

    if ( M_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS  ) {
        M_FIS->irs_belge_seri       = m_ui->lineEdit_irsaliye_belge_seri->text();
        M_FIS->irs_belge_numarasi   = m_ui->lineEdit_irsaliye_belge_numarasi->text();
    }

    int fis_id = FATURA_FISI_EKLE ( M_FIS );

    m_ui->lineEdit_fis_no->setText      ( QVariant ( M_FIS->fis_no ).toString() );

    m_log_detaylari = FAT_GET_FATURA_TURLERI_STRING(M_FIS->fatura_turu) + " , " + "Fiş no : " + m_ui->lineEdit_fis_no->text()
                      + " , " + "Fatura Tarihi : " + m_ui->adakDate_fatura_tarihi->QDATE().toString("dd MMMM YYYY")
                      + " , " + "Fatura : " + m_ui->lineEdit_belge_seri->text() +
                      " " + m_ui->lineEdit_belge_numarasi->text() + "\n";
    m_log_detaylari.append("Cari Hesap Kodu/Adı : " + m_ui->searchEdit_cari_hesap_kodu->GET_TEXT() + "  " +
                           m_ui->lineEdit_cari_hesap_ismi->text() + " , " + FAT_GET_ACIK_KAPALI_FATURA_STRING(M_FIS->acik_kapali_fatura) + "\n");

    m_log_kaydi_eklenecek_mi = true;
    m_kayit_eklendi          = true;

    return fis_id;
}

/**************************************************************************************
                   FAT_FATURA_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int FAT_FATURA_FISI::CHECK_FIS_RECORD_UPDATE ( int p_fis_id )
{
    SQL_QUERY query ( DB );


    if ( FATURADA_DEGISIKLIK_YAPILABILIR_MI ( p_fis_id ) EQ false ) {
        return ADAK_FAIL;
    }

    if ( QString ( m_ui->lineEdit_fis_no->text().at(0) ) EQ "*" ) {
        MSG_WARNING( tr("Fiş numarasını güncellemek için, fiş numarası kısmına sadece sayı girebilirsiniz."), m_ui->lineEdit_fis_no );

        return ADAK_FAIL;
    }

    int     fis_no              = m_ui->lineEdit_fis_no->text().toInt();
    QString fatura_tarihi       = m_ui->adakDate_fatura_tarihi->DATE();

    query.PREPARE_SELECT ( "fat_faturalar ","fis_id",
                           "fis_no        = :fis_no "
                           "AND fatura_tarihi   = :fatura_tarihi");

    query.SET_VALUE ( ":fis_no",        fis_no          );
    query.SET_VALUE ( ":fatura_tarihi", fatura_tarihi   );

    if ( query.SELECT() > 0 ) {
        query.NEXT();
        if ( query.VALUE(0).toInt() NE p_fis_id ) {
            MSG_WARNING( tr("Aynı fiş numarasıyla kayıtlı , fatura bulundu. Lütfen fiş numarasını değiştiriniz!.."), m_ui->lineEdit_fis_no );

            return ADAK_FAIL;
        }
    }

    switch ( M_FIS->fatura_turu ) {
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
        {

            query.PREPARE_SELECT("fat_faturalar,fat_irs_baglantisi" ,
                                 "fatura_tarihi, irsaliye_id",
                                 "fat_faturalar.fis_id    = fat_irs_baglantisi.fatura_id "
                                 "AND fat_faturalar.fis_id = :fis_id");

            query.SET_VALUE(":fis_id", p_fis_id );

            if ( query.SELECT() EQ 0 ) {
                return ADAK_FAIL;
            }

            query.NEXT();

            QDate       old_fatura_tarihi   =   QDate::fromString ( query.VALUE(0).toString(), "yyyy.MM.dd" );
            QDate       new_fatura_tarihi   =   m_ui->adakDate_fatura_tarihi->QDATE();
            int         irsaliye_id         =   query.VALUE(1).toInt();

            STK_IRS_FIS_STRUCT * IRS_FISI  = new STK_IRS_FIS_STRUCT;
            STK_IRS_CLEAR_FIS_STRUCT(IRS_FISI);

            STK_IRS_FIS_BILGILERINI_OKU(irsaliye_id , IRS_FISI);

            int         depo_id_in_db       =   IRS_FISI->depo_id;
            QString     depo_kodu, depo_adi ;

            SUBE_GET_DEPO_KODU_ADI( depo_id_in_db,  &depo_kodu, &depo_adi );

            if ( depo_id_in_db NE M_FIS->depo_id ) {
                MSG_WARNING(  tr("Bu fatura için depo kodu: " + depo_kodu.toUtf8() + ", depo_adı: " + depo_adi.toUtf8()  + " olan depo kaydedilmiş. Depo değişikliği yapamazsınız." ), NULL);
                return ADAK_FAIL;
            }

            if ( old_fatura_tarihi NE new_fatura_tarihi ) {
                if ( STK_IRSALIYE_TARIH_DEGISIKLIGINE_GORE_STOK_MIKTARLARI_KONTROLU ( irsaliye_id, old_fatura_tarihi, new_fatura_tarihi, depo_id_in_db, M_FIS->fatura_alis_satis_turu, this )  EQ false ) {
                    return ADAK_FAIL;
                }
            }

            break;
        }

        default :
            break;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void FAT_FATURA_FISI::UPDATE_FIS_RECORD ( int p_fis_id )
{

    FATURA_FIS_BILGILERINI_OKU(M_FIS,p_fis_id);

    M_FIS->fis_no                       = m_ui->lineEdit_fis_no->text().toInt();
    M_FIS->iskonto_yuzdesi              = m_ui->commaEdit_iskonto_yuzdesi->GET_DOUBLE();
    M_FIS->fatura_tarihi                = m_ui->adakDate_fatura_tarihi->DATE() ;
    M_FIS->acik_kapali_fatura           = FAT_GET_ACIK_KAPALI_FATURA_ENUM ( m_ui->comboBox_acik_kapali_fatura->currentText() );
    M_FIS->belge_numarasi               = m_ui->lineEdit_belge_numarasi->text();
    M_FIS->belge_seri                   = m_ui->lineEdit_belge_seri->text();
    M_FIS->cari_hesap_ismi              = m_ui->lineEdit_cari_hesap_ismi->text();
    M_FIS->aciklama                     = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->cari_hesap_adresi            = m_ui->limitedTextEdit_adres->toPlainText();
    M_FIS->vergi_dairesi                = m_ui->comboBox_vergi_dairesi->currentText();
    M_FIS->vergi_numarasi               = m_ui->lineEdit_vergi_no->text();
    M_FIS->ara_toplam                   = m_ui->commaEdit_ara_toplam->GET_DOUBLE();
    M_FIS->isk_sonrasi_otv_dahil_tutar  = m_ui->commaEdit_isk_sonrasi_otv_tutari->GET_DOUBLE();
    M_FIS->isk_sonrasi_otv_dahil_tutar  = m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->GET_DOUBLE();
    M_FIS->isk_sonrasi_kdv_tutari       = m_ui->commaEdit_isk_sonrasi_kdv_tutari->GET_DOUBLE();
    M_FIS->fatura_tutari                = m_ui->commaEdit_fatura_tutari->GET_DOUBLE();
    M_FIS->iskonto_yuzdesi              = m_ui->commaEdit_iskonto_yuzdesi->GET_DOUBLE();
    M_FIS->iskonto_tutari               = m_ui->commaEdit_iskonto_tutari->GET_DOUBLE();
    M_FIS->doviz_kuru                   = m_ui->commaEdit_kur->GET_DOUBLE();
    M_FIS->parite                       = m_ui->commaEdit_parite->GET_DOUBLE();
    M_FIS->cari_para_birimi_tutari      = m_ui->commaEdit_cari_para_birimi_tutari->GET_DOUBLE();
    M_FIS->modul_id                     = FATURA_MODULU;
    M_FIS->program_id                   = E9_PROGRAMI;
    M_FIS->irs_belge_seri               = m_ui->lineEdit_irsaliye_belge_seri->text();
    M_FIS->irs_belge_numarasi           = m_ui->lineEdit_irsaliye_belge_numarasi->text();
    M_FIS->fatura_para_birimi_id        = DVZ_GET_DOVIZ_ID( m_ui->comboBox_fatura_para_birimi_id->currentText() );

    FATURA_FISI_GUNCELLE ( M_FIS, p_fis_id );

    SATIR_DOVIZ_TUTARLARINI_YAZDIR();
    OTV_ORANLARI_BILGILERINI_YAZDIR();
    KDV_ORANLARI_BILGILERINI_YAZDIR();

    M_FATURA_DETAYLARI->muh_fis_no  =  M_FIS->muh_fis_no;
    M_FATURA_DETAYLARI->cari_fis_no =  M_FIS->cari_fis_no;

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = FAT_GET_FATURA_TURLERI_STRING ( M_FIS->fatura_turu ) + " , " + "Fatura No : " + QVariant ( M_FIS->fis_no ).toString() + " , " +
                      "Fatura Tarihi : " + m_ui->adakDate_fatura_tarihi->QDATE().toString("dd MMMM YYYY")+ " , " +
                      "Fatura : "        + m_ui->lineEdit_belge_seri->text() + " " + m_ui->lineEdit_belge_numarasi->text() + "\n";

    m_log_detaylari.append ( "Cari Hesap Kodu/Adı : " + m_ui->searchEdit_cari_hesap_kodu->GET_TEXT() + "  " +
                             m_ui->lineEdit_cari_hesap_ismi->text() + " , " + FAT_GET_ACIK_KAPALI_FATURA_STRING ( M_FIS->acik_kapali_fatura ) + "\n");

    m_log_detaylari.append ( "Ara Toplam : "      + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_ara_toplam->GET_DOUBLE() ).toString(), 2 ) + " , " +
                             "Kdv Tutarı : "      + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_kdv_tutari->GET_DOUBLE() ).toString(),         2 ) + " , " +
                             "Fatura Tutarı : "   + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_fatura_tutari->GET_DOUBLE() ).toString(),   2 ) + "\n" );
    m_log_detaylari.append ( "Ötv Tutarı : "      + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_tutari->GET_DOUBLE() ).toString(),         2 ) + " , " +
                             "Ötv Dahil Tutar : " + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->GET_DOUBLE() ).toString(),   2 ) );

}

/**************************************************************************************
                   FAT_FATURA_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int FAT_FATURA_FISI::CHECK_FIS_RECORD_DELETE ( int p_fis_id )
{

    if ( FATURADA_DEGISIKLIK_YAPILABILIR_MI ( p_fis_id, 1 ) EQ false ) {
        return ADAK_FAIL;
    }

    SQL_QUERY query ( DB );

    QMessageBox uyari       ( this );
    uyari.setWindowTitle    ( tr("Uyarı") );

    if ( M_FIS->yazdirildi_mi EQ 1 ) {
        uyari.setText(tr ( "Fatura yazdırıldığı için silinemez, iptal işlemi gerçekleştirilecektir.\n "
                           "Devam etmek istiyor musunuz?" ) );

        uyari.setIcon           ( QMessageBox::Warning                  );
        uyari.addButton         ( QMessageBox::Ok                       );
        uyari.addButton         ( QMessageBox::Cancel                   );
        uyari.setButtonText     ( QMessageBox::Ok, tr ( "Tamam" )       );
        uyari.setButtonText     ( QMessageBox::Cancel, tr ( "Vazgeç" )  );
        uyari.exec              ();

        if ( uyari.standardButton ( uyari.clickedButton()) EQ QMessageBox::Cancel ) {
            return ADAK_FAIL;
        }
    }

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI :
        {
            query.PREPARE_SELECT("fat_irs_baglantisi","irsaliye_id" ,"fatura_id = :fatura_id");
            query.SET_VALUE(":fatura_id", p_fis_id );

            if ( query.SELECT() EQ 0 ) {
                return ADAK_FAIL;
            }

            query.NEXT();

            int irsaliye_id = query.VALUE(0).toInt();

            if ( STK_IRSALIYE_SILINMESI_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU ( irsaliye_id, this ) EQ false ) {
                return ADAK_FAIL;
            }

            break;
        }

        default :
            break;

    }

    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void FAT_FATURA_FISI::DELETE_FIS_RECORD ( int p_fis_id )
{
    FATURA_FISI_SIL ( M_FIS, p_fis_id );

    QString cari_hesap_kodu, cari_hesap_ismi;

    if ( M_FIS->cari_hesap_id > 0 ) {      
        CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( M_FIS->cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );
    }

    m_log_kaydi_eklenecek_mi = true;
    m_kayit_silindi          = true;

    m_log_detaylari = FAT_GET_FATURA_TURLERI_STRING ( M_FIS->fatura_turu ) + " , " + "Fiş No : " + QVariant ( M_FIS->fis_no ).toString() + " , " +
                      "Fatura Tarihi : " + QDate::fromString ( M_FIS->fatura_tarihi, "yyyy.MM.dd" ).toString ( "dd MMMM yyyy dddd" ) + " , " +
                      "Fatura : "        + M_FIS->belge_seri + " " + M_FIS->belge_numarasi + "\n";

    m_log_detaylari.append ( "Cari Hesap Kodu/Adı : " + cari_hesap_kodu + "  " + cari_hesap_ismi + " , " +
                             FAT_GET_ACIK_KAPALI_FATURA_STRING ( M_FIS->acik_kapali_fatura ) + "\n");

    m_log_detaylari.append ( "Ara Toplam : "      +  VIRGUL_EKLE ( QVariant ( M_FIS->ara_toplam                  ).toString(), 2 ) + " , " +
                             "Kdv Tutarı : "      +  VIRGUL_EKLE ( QVariant ( M_FIS->isk_sonrasi_kdv_tutari      ).toString(), 2 ) + " , " +
                             "Fatura Tutarı : "   +  VIRGUL_EKLE ( QVariant ( M_FIS->fatura_tutari               ).toString(), 2 ) + "\n" );

    m_log_detaylari.append ( "Ötv Tutarı : "      +  VIRGUL_EKLE ( QVariant ( M_FIS->isk_sonrasi_otv_tutari      ).toString(), 2 ) + " , " +
                             "Ötv Dahil Tutar : " +  VIRGUL_EKLE ( QVariant ( M_FIS->isk_sonrasi_otv_dahil_tutar ).toString(), 2 ) );
}

/**************************************************************************************
                   FAT_FATURA_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int FAT_FATURA_FISI::SELECT_FIS_RECORD()
{
    return OPEN_FATURA_ARAMA ( M_FIS->fatura_alis_satis_turu, m_iade_faturasi_mi, this );
}

/**************************************************************************************
                   FAT_FATURA_FISI::FIND_FIS_RECORD
***************************************************************************************/

int FAT_FATURA_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("fat_faturalar","fis_id"," fis_no = :fis_no "
                                "AND fatura_turu       = :fatura_turu "
                                "AND fatura_tarihi     = :fatura_tarihi " );

    select_query.SET_VALUE(":fis_no"          , m_ui->lineEdit_fis_no->text().toInt());
    select_query.SET_VALUE(":fatura_turu"     , M_FIS->fatura_turu);
    select_query.SET_VALUE(":fatura_tarihi"   , m_ui->adakDate_fatura_tarihi->DATE());

    if ( select_query.SELECT() EQ 0 ) {
        MSG_WARNING( tr("Aradığınız fatura bulunamadı!.."), m_ui->lineEdit_fis_no );

        return ADAK_FAIL;
    }
    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   FAT_FATURA_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int FAT_FATURA_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT("fat_faturalar" , "fis_id" , " fatura_turu = :fatura_turu ");
    select_query.SET_VALUE ( ":fatura_turu" , M_FIS->fatura_turu );

    select_query.AND_EKLE   ( " iade_faturasi_mi = :iade_faturasi_mi ");
    select_query.SET_VALUE  ( ":iade_faturasi_mi", m_iade_faturasi_mi );

    if ( select_query.SELECT("fatura_tarihi ASC , fis_no ASC" , 0, 1) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   FAT_FATURA_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int FAT_FATURA_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT("fat_faturalar" , "fis_id" , " fatura_turu = :fatura_turu ");

    select_query.SET_VALUE ( ":fatura_turu" , M_FIS->fatura_turu );

    select_query.AND_EKLE   ( " iade_faturasi_mi = :iade_faturasi_mi ");
    select_query.SET_VALUE  ( ":iade_faturasi_mi", m_iade_faturasi_mi );

    if ( select_query.SELECT("fatura_tarihi DESC , fis_no DESC" , 0, 1) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   FAT_FATURA_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int FAT_FATURA_FISI::FIND_PREV_FIS_RECORD ()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("fat_faturalar" , "fis_id",
                                "fis_no < :fis_no "
                                "AND fatura_turu    = :fatura_turu   "
                                "AND fatura_tarihi  = :fatura_tarihi ");

    select_query.SET_VALUE(":fis_no"          , m_ui->lineEdit_fis_no->text().toInt());
    select_query.SET_VALUE(":fatura_turu"     , M_FIS->fatura_turu);
    select_query.SET_VALUE(":fatura_tarihi"   , m_ui->adakDate_fatura_tarihi->DATE());

    select_query.AND_EKLE   ( " iade_faturasi_mi = :iade_faturasi_mi ");
    select_query.SET_VALUE  ( ":iade_faturasi_mi", m_iade_faturasi_mi );

    if ( select_query.SELECT("fis_no DESC" , 0,1) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }


    select_query.PREPARE_SELECT("fat_faturalar" , "fis_id" , " fatura_turu     = :fatura_turu   "
                                "AND fatura_tarihi     < :fatura_tarihi ");

    select_query.SET_VALUE ( ":fatura_turu"     , M_FIS->fatura_turu );
    select_query.SET_VALUE ( ":fatura_tarihi"   , m_ui->adakDate_fatura_tarihi->DATE());

    select_query.AND_EKLE   ( " iade_faturasi_mi = :iade_faturasi_mi ");
    select_query.SET_VALUE  ( ":iade_faturasi_mi", m_iade_faturasi_mi );

    if ( select_query.SELECT("fatura_tarihi DESC, fis_no DESC" , 0,1) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   FAT_FATURA_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int FAT_FATURA_FISI::FIND_NEXT_FIS_RECORD ()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("fat_faturalar" , "fis_id",
                                "fis_no > :fis_no                       "
                                "AND fatura_turu       = :fatura_turu   "
                                "AND fatura_tarihi     = :fatura_tarihi ");

    select_query.SET_VALUE(":fis_no"          , m_ui->lineEdit_fis_no->text().toInt());
    select_query.SET_VALUE(":fatura_turu"     , M_FIS->fatura_turu);
    select_query.SET_VALUE(":fatura_tarihi"   , m_ui->adakDate_fatura_tarihi->DATE());

    select_query.AND_EKLE   ( " iade_faturasi_mi = :iade_faturasi_mi ");
    select_query.SET_VALUE  ( ":iade_faturasi_mi", m_iade_faturasi_mi );

    if ( select_query.SELECT("fis_no ASC" , 0, 1) > 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }


    select_query.PREPARE_SELECT("fat_faturalar" , "fis_id" , " fatura_turu     = :fatura_turu   "
                                "AND fatura_tarihi     > :fatura_tarihi ");

    select_query.SET_VALUE ( ":fatura_turu"     , M_FIS->fatura_turu );
    select_query.SET_VALUE ( ":fatura_tarihi"   , m_ui->adakDate_fatura_tarihi->DATE());

    select_query.AND_EKLE   ( " iade_faturasi_mi = :iade_faturasi_mi ");
    select_query.SET_VALUE  ( ":iade_faturasi_mi", m_iade_faturasi_mi );

    if ( select_query.SELECT("fatura_tarihi ASC , fis_no ASC") EQ 0 ) {
        return 0;
    }

    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   FAT_FATURA_FISI::CHECK_LINE_VAR
***************************************************************************************/

int FAT_FATURA_FISI::CHECK_LINE_VAR ( int p_row_number, QObject * p_object )
{

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :

            if ( M_FIS->depo_id < 1 ) {
                QMessageBox::warning(this, tr("HATA"), tr("Önce depo seçimi yapmalısınız."));
                return ADAK_FAIL;
            }
            break;

        case ENUM_FAT_PROFORMA_SATIS_FATURASI   :
        case ENUM_FAT_VADE_FARKI_ALIS_FATURASI  :
        case ENUM_FAT_VADE_FARKI_SATIS_FATURASI :
        default                        :
            break;

    }

    QSearchEdit * searchEdit_urun_kodu                      = ( QSearchEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_KODU_COLUMN                 );
    QLineEdit   * lineEdit_urun_id                          = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN                   );
    QLineEdit   * lineEdit_urun_adi                         = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ADI_COLUMN                  );
    QCommaEdit  * commaEdit_iskonto_oncesi_birim_fiyat      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN    );
    QCommaEdit  * commaEdit_iskonto_sonrasi_birim_fiyat     = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN   );
    QCommaEdit  * commaEdit_iskonto_yuzdesi                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_YUZDESI_COLUMN           );
    QCommaEdit  * commaEdit_iskonto_tutari                  = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_TUTARI_COLUMN            );
    QLineEdit   * lineEdit_stok_takip_sekli                 = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, STOK_TAKIP_SEKLI_COLUMN          );
    QComboBox   * comboBox_urun_birimleri                   = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN               );
    QPushButton * pushButton_seri_parti_kodu                = ( QPushButton * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SERI_PARTI_NO_KODU_COLUMN        );
    QCommaEdit  * commaEdit_satirdaki_urun_miktari          = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIRDAKI_URUN_MIKTARI_COLUMN    );
    QCommaEdit  * commaEdit_satir_tutari                    = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN              );
    QLineEdit   * lineEdit_parti_kodu_id                    = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, PARTI_ID_COLUMN                  );
    QComboBox   * comboBox_kdv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_ORANI_COLUMN                 );
    QComboBox   * comboBox_kdv_dahil_mi                     = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN              );
    QCommaEdit  * commaEdit_kdv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_TUTAR_COLUMN           );
    QCommaEdit  * commaEdit_kdv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_TUTARI_COLUMN                );
    QComboBox   * comboBox_otv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_ORANI_COLUMN                 );
    QCommaEdit  * commaEdit_vergi_haric_tutar               = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTAR_COLUMN         );
    QCommaEdit  * commaEdit_otv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_DAHIL_TUTAR_COLUMN           );
    QCommaEdit  * commaEdit_otv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_TUTARI_COLUMN                );
    QLineEdit   * lineEdit_seri_no_list                     = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SERI_NO_LIST_COLUMN              );
    QLineEdit   * lineEdit_makbuz_para_birimi               = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, FATURA_PARA_BIRIMI_COLUMN            );

    int urun_id = 0;

    if ( p_object EQ searchEdit_urun_kodu ) {

        if ( searchEdit_urun_kodu->GET_TEXT().isEmpty() EQ true ) {

            lineEdit_urun_id->setText                       ( "-1" );
            lineEdit_urun_adi->clear                        ();
            lineEdit_urun_adi->setReadOnly                  ( false );
            comboBox_urun_birimleri->clear                  ();
            commaEdit_satir_tutari->clear                   ();
            commaEdit_satirdaki_urun_miktari->clear         ();
            commaEdit_iskonto_oncesi_birim_fiyat->clear     ();
            commaEdit_iskonto_sonrasi_birim_fiyat->clear    ();
            commaEdit_iskonto_yuzdesi->clear                ();
            commaEdit_iskonto_tutari->clear                 ();
            lineEdit_stok_takip_sekli->setText              ( "-1" );
            lineEdit_seri_no_list->clear();
            pushButton_seri_parti_kodu->setEnabled          ( true );
            lineEdit_parti_kodu_id->setText                 ( "-1" );
            comboBox_kdv_orani->setCurrentIndex             ( 0 );
            comboBox_kdv_dahil_mi->setCurrentIndex          ( 0 );
            commaEdit_kdv_tutari->clear                     ();
            commaEdit_kdv_dahil_tutar->clear                ();
            comboBox_otv_orani->setCurrentIndex             ( 0 );
            commaEdit_vergi_haric_tutar->clear              ();
            commaEdit_otv_tutari->clear                     ();
            commaEdit_otv_dahil_tutar->clear                ();
        }

        switch ( M_FIS->fatura_turu ) {

            case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
                // Bu fatura turleri icin stoktan direkt
                //azaltma yapilmasi gerekiyor bu yuzden depoya bakarak
                // urun depoda var mi tespiti yapilmali

                urun_id = STK_GET_URUN_ID(searchEdit_urun_kodu->GET_TEXT() ,M_FIS->depo_id );

                if ( STK_GET_HIZMET_MI( searchEdit_urun_kodu->GET_TEXT(), &urun_id ) NE 1 ) {
                    if ( urun_id EQ 0 ) {
                       MSG_WARNING( tr("Seçili depoda bu ürün bulunamadı."), searchEdit_urun_kodu );
                       return ADAK_FAIL_UNDO;
                    }
                }

                break;

            case ENUM_FAT_HIZMET_ALIS_FATURASI  :
            case ENUM_FAT_HIZMET_SATIS_FATURASI :

                if ( M_FIS->fatura_turu EQ ENUM_FAT_HIZMET_ALIS_FATURASI ) {
                    urun_id = STK_GET_URUN_ID(searchEdit_urun_kodu->GET_TEXT() , -1 , 1 , ENUM_ALINAN_SATILAN_HIZMET);
                }
                else {
                    urun_id = STK_GET_URUN_ID(searchEdit_urun_kodu->GET_TEXT() , -1 , 1 , ENUM_ALINAN_SATILAN_HIZMET );
                }

                if ( urun_id EQ 0 ) {
                    lineEdit_urun_id->setText("0");
                    lineEdit_urun_adi->setReadOnly(false);
                    MSG_WARNING( tr("Aradığınız hizmet bulunamadı!.."), searchEdit_urun_kodu );

                    return ADAK_FAIL_UNDO;
                }
                break;
            default :

                urun_id = STK_GET_URUN_ID(searchEdit_urun_kodu->GET_TEXT());


                if ( urun_id EQ 0 ) {
                    lineEdit_urun_id->setText("0");
                    lineEdit_urun_adi->setReadOnly(false);
                    MSG_WARNING( tr("Aradığınız ürün bulunamadı!.."), searchEdit_urun_kodu );

                    return ADAK_FAIL_UNDO;
                }
                break;
        }

        STK_URUNLER_STRUCT * URUN = new STK_URUNLER_STRUCT;

        STK_URUNLER_TABLOSUNU_OKU(urun_id , URUN);

        int stok_takip_sekli  = URUN->stok_takip_sekli;

        lineEdit_urun_id->setText           ( QVariant ( urun_id ).toString() );
        lineEdit_urun_adi->setText          ( URUN->urun_adi );
        lineEdit_urun_adi->setReadOnly      ( true );
        lineEdit_stok_takip_sekli->setText  ( QVariant ( stok_takip_sekli ).toString() );

        if ( M_FIS->fatura_alis_satis_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI ) {
            comboBox_kdv_orani->setCurrentIndex ( comboBox_kdv_orani->findText ( QVariant( URUN->alis_kdv_orani ).toString()));
            comboBox_otv_orani->setCurrentIndex ( comboBox_otv_orani->findText ( QVariant( URUN->alis_otv_orani ).toString()));
        }
        else {
            comboBox_kdv_orani->setCurrentIndex ( comboBox_kdv_orani->findText ( QVariant( URUN->satis_kdv_orani ).toString()));
            comboBox_otv_orani->setCurrentIndex ( comboBox_otv_orani->findText ( QVariant( URUN->satis_otv_orani ).toString()));
        }

        commaEdit_satirdaki_urun_miktari->setDisabled   ( false );
        if ( m_fatura_turu NE ENUM_FAT_HIZMET_ALIS_FATURASI AND
             m_fatura_turu NE ENUM_FAT_HIZMET_SATIS_FATURASI ) {
            comboBox_urun_birimleri->setEnabled             ( true  );
        }

        switch ( stok_takip_sekli ) {

            case ENUM_BASIT_TAKIP :
            default          :

                pushButton_seri_parti_kodu->setEnabled ( false );
                lineEdit_seri_no_list->clear();
                break;

            case ENUM_SERI_NO_ILE_TAKIP :

                if ( M_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_ALIS_FATURASI OR M_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI OR
                     M_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS OR M_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS ) {
                    commaEdit_satirdaki_urun_miktari->setDisabled ( true );
                }
                comboBox_urun_birimleri->setEnabled  ( false );
                pushButton_seri_parti_kodu->setEnabled(true);
                break;

            case ENUM_PARTI_TAKIBI      :
                pushButton_seri_parti_kodu->setEnabled(true);
                lineEdit_seri_no_list->clear();
                break;
        }

        lineEdit_makbuz_para_birimi->setText( DVZ_GET_DOVIZ_KODU( M_FIS->fatura_para_birimi_id ) );

        STK_FILL_URUN_BIRIMLERI_COMBOBOX ( urun_id, comboBox_urun_birimleri, M_STOK_BIRIM_ADLARI );
        URUN_BILGILERINI_AL              ( p_row_number );
        URUN_BIRIMI_FIYATLARINI_HESAPLA  ( p_row_number );

        double vergi_haric_tutar = commaEdit_satir_tutari->GET_DOUBLE();

        commaEdit_vergi_haric_tutar->SET_DOUBLE ( ROUND ( vergi_haric_tutar ) );

        double otv_orani = comboBox_otv_orani->currentText().toDouble();
        double kdv_orani = comboBox_kdv_orani->currentText().toDouble();

        commaEdit_otv_tutari->SET_DOUBLE ( ROUND ( vergi_haric_tutar * ( otv_orani / 100.0 )  ) );

        double otv_tutari       = commaEdit_otv_tutari->GET_DOUBLE();

        commaEdit_otv_dahil_tutar->SET_DOUBLE ( ROUND ( vergi_haric_tutar + otv_tutari ) );

        double otv_dahil_tutar  = commaEdit_otv_dahil_tutar->GET_DOUBLE();

        commaEdit_kdv_tutari->SET_DOUBLE ( ROUND ( otv_dahil_tutar * ( kdv_orani / 100.0 ) ) );

        double kdv_tutari       = commaEdit_kdv_tutari->GET_DOUBLE();

        commaEdit_kdv_dahil_tutar->SET_DOUBLE ( ROUND ( otv_dahil_tutar + kdv_tutari ) );

        TOPLU_ISKONTO_SONRASI_SATIR_DEGERLERINI_HESAPLA ( p_row_number );

        //! Odaklanma
        if ( M_FIS->fatura_turu NE ENUM_FAT_VADE_FARKI_ALIS_FATURASI OR
             M_FIS->fatura_turu NE ENUM_FAT_VADE_FARKI_SATIS_FATURASI ) {
            SET_FOCUS( commaEdit_satirdaki_urun_miktari );
        }

    }
    if ( ( p_object EQ comboBox_urun_birimleri) OR ( p_object EQ commaEdit_satirdaki_urun_miktari )
           OR ( p_object EQ commaEdit_satir_tutari ) OR ( p_object EQ commaEdit_iskonto_oncesi_birim_fiyat )
           OR ( p_object EQ commaEdit_iskonto_sonrasi_birim_fiyat ) OR ( p_object EQ commaEdit_iskonto_yuzdesi )
           OR ( p_object EQ lineEdit_makbuz_para_birimi ) OR ( p_object EQ comboBox_kdv_orani )
           OR ( p_object EQ comboBox_kdv_dahil_mi ) OR ( p_object EQ comboBox_otv_orani ) ) {

        return FATURA_SATIR_DEGERLERI_HESAPLAMASI( p_row_number, p_object );
    }
    else if ( p_object EQ pushButton_seri_parti_kodu ) {

        switch ( M_FIS->fatura_turu ) {

            case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS :
            case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
            {
                if ( lineEdit_seri_no_list->text().isEmpty() EQ true ) {
                    lineEdit_parti_kodu_id->clear();
                    return ADAK_OK;
                }

                int stok_takip_sekli = lineEdit_stok_takip_sekli->text().toInt();

                if ( stok_takip_sekli EQ ENUM_PARTI_TAKIBI ) {
                    int parti_id = STK_GET_PARTI_ID( lineEdit_seri_no_list->text() );

                    if ( parti_id EQ 0 ) {
                        MSG_WARNING( tr("Parti kodu bulunamadı."), pushButton_seri_parti_kodu );

                        return ADAK_FAIL_UNDO;
                    }

                    lineEdit_parti_kodu_id->setText(QVariant(parti_id).toString());
                }
            }

            default :
                break;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int FAT_FATURA_FISI::CHECK_LINE_EMPTY ( int p_row_number )
{
    QLineEdit   * lineedit_stok_takip_sekli   = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number,    STOK_TAKIP_SEKLI_COLUMN     );
    QPushButton * pushButton_seri_parti_kodu  = ( QPushButton * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number,    SERI_PARTI_NO_KODU_COLUMN   );
    QLineEdit   * lineedit_urun_adi           = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number,    URUN_ADI_COLUMN             );
    QLineEdit   * lineEdit_irsaliye_satiri_id = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number,    IRSALIYE_SATIRI_ID_COLUMN   );
    QLineEdit   * lineEdit_seri_no_list       = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number,    SERI_NO_LIST_COLUMN         );

    QCommaEdit  * commaEdit_satir_urun_miktari= ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number,    SATIRDAKI_URUN_MIKTARI_COLUMN );
    QCommaEdit  * commaEdit_isk_oncesi_birim_fiyat = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number,    ISK_ONCESI_BIRIM_FIYAT_COLUMN );

    int urun_stok_takip_sekli = lineedit_stok_takip_sekli->text().toInt();


    if ( M_FIS->fatura_para_birimi_id EQ -1 ) {
        MSG_WARNING( "Fatura Makbuz Para Birimini Seçmediniz.!", m_ui->comboBox_fatura_para_birimi_id );
        return ADAK_FAIL;
    }

    if ( lineedit_urun_adi->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Hizmet / Ürün adını boş bırakamazsınız."), lineedit_urun_adi );

        return ADAK_FAIL;
    }

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS  :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS : {
            int irsaliye_satiri_id = lineEdit_irsaliye_satiri_id->text().toInt();
            if ( irsaliye_satiri_id < 1 ) {
                MSG_WARNING( tr("Satırı kaydedebilmek için uygun bir irsaliye satırı seçmelisiniz."), lineEdit_irsaliye_satiri_id );
                return ADAK_FAIL;
            }
        }
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS :
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS  :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI   :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI  :

            switch ( urun_stok_takip_sekli) {

                case ENUM_PARTI_TAKIBI      :
                    if ( lineEdit_seri_no_list->text().isEmpty() EQ true ) {
                        MSG_WARNING( tr("Ürün parti numarasına göre takip edilmektedir.Parti kodunu boş bırakamazsınız."), pushButton_seri_parti_kodu );
                        return ADAK_FAIL;
                    }
                    break;
                default :
                    break;
            }
        default :
            break;
    }

    //! HATALI FIS OLUSMAMASI ICIN KONTROL

    switch( M_FIS->fatura_turu ) {

        case ENUM_FAT_VADE_FARKI_ALIS_FATURASI  :
        case ENUM_FAT_VADE_FARKI_SATIS_FATURASI :
        // vade farki kontrol
        break;

        default:
            if ( commaEdit_satir_urun_miktari->GET_DOUBLE() EQ 0.00 ) {
                MSG_WARNING( "Ürün miktarı boş geçilemez.!", NULL );
                if ( urun_stok_takip_sekli EQ ENUM_BASIT_TAKIP ) {
                    SET_FOCUS( commaEdit_satir_urun_miktari );
                }
                else {
                    SET_FOCUS( pushButton_seri_parti_kodu );
                }
                return ADAK_FAIL;
            }
            else if ( commaEdit_isk_oncesi_birim_fiyat->GET_DOUBLE() EQ 0.00 ) {
                MSG_WARNING( "Ürün Birim Fiyatı boş geçilemez.!", commaEdit_isk_oncesi_birim_fiyat );
                return ADAK_FAIL;
            }
            break;
    }


    return ADAK_OK;
}


/**************************************************************************************
                   FAT_FATURA_FISI::CHECK_ADD_LINE
***************************************************************************************/

int FAT_FATURA_FISI::CHECK_ADD_LINE ( int p_fis_id, int p_row_number )
{
    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS           :
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS          :           
            MSG_WARNING(  tr("Otomatik irsaliye faturalaştırma yaparken, elle satır giremezsiniz."), m_ui->lineEdit_fis_no );
            return ADAK_FAIL;

        default :
            break;
    }

    if ( FATURADA_DEGISIKLIK_YAPILABILIR_MI ( p_fis_id ) EQ false ) {
        return ADAK_FAIL;
    }

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :

            if ( M_FIS->depo_id < 1 ) {
                MSG_WARNING( tr("Önce depo seçimi yapmalısınız."), m_ui->searchEdit_depo_kodu );

                return ADAK_FAIL;
            }
            break;

        case ENUM_FAT_PROFORMA_SATIS_FATURASI   :
        case ENUM_FAT_VADE_FARKI_ALIS_FATURASI  :
        case ENUM_FAT_VADE_FARKI_SATIS_FATURASI :
        default                                 :
            break;
    }

    SQL_QUERY sql_query ( DB );


    QLineEdit   * lineEdit_urun_id                      = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN                   );
    QCommaEdit  * commaEdit_satirdaki_urun_miktari      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIRDAKI_URUN_MIKTARI_COLUMN    );
    QCommaEdit  * commaEdit_temel_birim_katsayisi       = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, TEMEL_BIRIM_KATSAYISI_COLUMN     );
    QCommaEdit  * commaEdit_iskonto_oncesi_birim_fiyat  = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN     );
    QLineEdit   * lineEdit_stok_takip_sekli             = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, STOK_TAKIP_SEKLI_COLUMN          );
    QLineEdit   * lineEdit_parti_kodu_id                = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, PARTI_ID_COLUMN                  );
    QLineEdit   * lineEdit_irsaliye_satiri_id           = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, IRSALIYE_SATIRI_ID_COLUMN        );
    QLineEdit   * lineEdit_iade_edilen_fatura_satiri_id = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, IADE_EDILEN_FAT_SATIR_ID_COLUMN  );
    QComboBox   * combo_box_urun_birimi                 = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN );

    int           urun_id                           = lineEdit_urun_id->text().toInt();

    if ( urun_id > 0 ) {
        if ( commaEdit_iskonto_oncesi_birim_fiyat->GET_DOUBLE() EQ 0.00 ) {
            MSG_WARNING( tr( "Ürün / Hizmet birim fiyatını girmelisiniz..." ), commaEdit_iskonto_oncesi_birim_fiyat );
            return ADAK_FAIL;
        }
    }
    if ( urun_id < 1 ) {
        return ADAK_OK;
    }

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS  :
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS  :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
        {

            sql_query.PREPARE_SELECT("fat_irs_baglantisi ","irsaliye_id",
                                     "fatura_id = :fatura_id");

            sql_query.SET_VALUE(":fatura_id", p_fis_id );
            if ( sql_query.SELECT() EQ 0 ) {
               MSG_WARNING(  tr("İrsaliyeli faturaya ait irsaliye bulunamadı."), NULL );
                return ADAK_FAIL;
            }

            sql_query.NEXT();

            int     irsaliye_id           = sql_query.VALUE(0).toInt();
            int     stok_takip_sekli      = lineEdit_stok_takip_sekli->text().toInt();
            double  girilen_stok_miktari  = commaEdit_satirdaki_urun_miktari->GET_DOUBLE() * commaEdit_temel_birim_katsayisi->GET_DOUBLE();

            STK_IRS_FIS_STRUCT * IRS_FISI = new STK_IRS_FIS_STRUCT;
            STK_IRS_CLEAR_FIS_STRUCT(IRS_FISI);

            int result = STK_IRS_FIS_BILGILERINI_OKU(irsaliye_id , IRS_FISI) ;

            if ( result EQ 0 ) {
               MSG_WARNING(  tr("Fiş bilgileri bulunamadı."), NULL );
               return ADAK_FAIL;
            }
            sql_query.NEXT();


            if ( IRS_GET_IRSALIYE_YAZDIRILDIMI( irsaliye_id ) EQ true ) {
                MSG_WARNING( tr( "İrsaliye Yazdırıldığı için fiş üzerinde değişiklik yapamazsınız.!"), NULL );
                return ADAK_FAIL;
            }


            QDate   fis_tarihi             = QDate::fromString ( IRS_FISI->fis_tarihi, "yyyy.MM.dd");
            int     depo_id                = IRS_FISI->depo_id;
            int     parti_id               = lineEdit_parti_kodu_id->text().toInt();

            delete IRS_FISI;

            //! ASAGIDA IADE FATURALARI ICIN KONTROL VAR BURDA SADECE IADE OLMAYANLARIN KONTROLU YAPILIYOR
            if( M_FIS->iade_faturasi_mi NE ENUM_FAT_IADE ) {
                if ( STK_IRSALIYEYE_SATIR_EKLENMESI_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU ( fis_tarihi, M_FIS->fatura_alis_satis_turu, stok_takip_sekli,
                                                                                           urun_id, depo_id, parti_id, girilen_stok_miktari,
                                                                                           combo_box_urun_birimi->currentText(), this ) EQ false ) {
                return ADAK_FAIL;
                }
            }
            break;
        }

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS           :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS          :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS  :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS :
        {

            int irsaliye_satiri_id = lineEdit_irsaliye_satiri_id->text().toInt();

            STK_IRS_FIS_SATIR_STRUCT * IRS_FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

            STK_IRS_CLEAR_SATIR_STRUCT(IRS_FIS_SATIRI);

            int result = STK_IRS_SATIR_BILGILERINI_OKU(irsaliye_satiri_id , IRS_FIS_SATIRI);

            if ( result EQ 0 ) {
                MSG_WARNING(  tr("Seçilmiş olan irsaliye satırının bilgileri okunamadı"), lineEdit_irsaliye_satiri_id );

                return ADAK_FAIL;
            }


            double irs_satirindaki_urun_miktari = IRS_FIS_SATIRI->satirdaki_urun_miktari;
            double irs_faturalanan_miktar       = IRS_FIS_SATIRI->faturalanan_miktar;
            double kalan_miktar                 = irs_satirindaki_urun_miktari - irs_faturalanan_miktar;
            double faturalanacak_miktar         = commaEdit_satirdaki_urun_miktari->GET_DOUBLE();

            delete IRS_FIS_SATIRI;

            if ( faturalanacak_miktar > kalan_miktar ) {
                QMessageBox::critical ( this, tr("FATURALANACAK MİKTAR AŞILDI"), tr("Seçili irsaliye satırında faturalanmamış ürün miktarı ") + QVariant( kalan_miktar ).toString().toUtf8() + tr(". Bu miktarı aşamazsınız.") );
                return ADAK_FAIL;
            }
        }

        default :
            break;

    }

    if ( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
        if ( M_FIS->fatura_turu NE ENUM_FAT_HIZMET_SATIS_FATURASI AND
             M_FIS->fatura_turu NE ENUM_FAT_HIZMET_ALIS_FATURASI ) {

            if ( FAT_IADE_FATURA_SATIR_EKLE_ICIN_MIKTAR_KONTROLU ( lineEdit_iade_edilen_fatura_satiri_id->text().toInt(), commaEdit_satirdaki_urun_miktari->GET_DOUBLE(), this ) EQ false  ) {
                return ADAK_FAIL;
            }
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_FISI::ADD_LINE
***************************************************************************************/

void FAT_FATURA_FISI::ADD_LINE ( int p_fis_id, int p_row_number )
{

    QLineEdit   * lineEdit_urun_id                          = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN                        );
    QLineEdit   * lineEdit_urun_adi                         = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ADI_COLUMN                       );
    QCommaEdit  * commaEdit_iskonto_oncesi_birim_fiyat      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN         );
    QCommaEdit  * commaEdit_iskonto_sonrasi_birim_fiyat     = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN        );
    QCommaEdit  * commaEdit_iskonto_yuzdesi                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_YUZDESI_COLUMN                );
    QCommaEdit  * commaEdit_iskonto_tutari                  = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_TUTARI_COLUMN                 );
    QComboBox   * comboBox_urun_birimleri                   = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN                    );
    QCommaEdit  * commaEdit_satirdaki_urun_miktari          = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIRDAKI_URUN_MIKTARI_COLUMN         );
    QCommaEdit  * commaEdit_satir_tutari                    = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN                   );
    QLineEdit   * lineEdit_parti_id                         = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, PARTI_ID_COLUMN                       );
    QLineEdit   * lineedit_para_birimi                      = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, FATURA_PARA_BIRIMI_COLUMN             );
    QComboBox   * comboBox_kdv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_ORANI_COLUMN                      );
    QComboBox   * comboBox_kdv_dahil_haric                  = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN                   );
    QCommaEdit  * commaEdit_kdv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_TUTAR_COLUMN                );
    QCommaEdit  * commaEdit_kdv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_TUTARI_COLUMN                     );
    QCommaEdit  * commaEdit_vergi_haric_tutar               = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTAR_COLUMN              );
    QComboBox   * comboBox_otv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_ORANI_COLUMN                      );
    QCommaEdit  * commaEdit_otv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_TUTARI_COLUMN                     );
    QCommaEdit  * commaEdit_otv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_DAHIL_TUTAR_COLUMN                );
    QCommaEdit  * commaEdit_temel_birim_katsayisi           = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, TEMEL_BIRIM_KATSAYISI_COLUMN          );
    //QPushButton * pushButton_seri_parti_kodu                = ( QPushButton * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SERI_PARTI_NO_KODU_COLUMN            );
    QLineEdit   * lineEdit_irsaliye_satiri_id               = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, IRSALIYE_SATIRI_ID_COLUMN             );
    QLineEdit   * lineEdit_iade_edilen_fat_satiri_id        = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, IADE_EDILEN_FAT_SATIR_ID_COLUMN       );
    QCommaEdit  * commaEdit_toplu_iskonto_yuzdesi           = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, TOPLU_ISKONTO_YUZDESI_COLUMN          );
    QCommaEdit  * commaEdit_t_is_sonrasi_birim_fiyat        = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_BIRIM_FIYAT_COLUMN       );
    QCommaEdit  * commaEdit_t_is_sonrasi_satir_tutari       = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_SATIR_TUTARI_COLUMN      );
    QCommaEdit  * commaEdit_t_is_sonrasi_vergi_haric_tutar  = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN );
    QCommaEdit  * commaEdit_t_is_sonrasi_otv_tutari         = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_OTV_TUTARI_COLUMN        );
    QCommaEdit  * commaEdit_t_is_sonrasi_otv_dahil_tutar    = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN   );
    QCommaEdit  * commaEdit_t_is_sonrasi_kdv_tutari         = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_KDV_TUTARI_COLUMN        );
    QCommaEdit  * commaEdit_t_is_sonrasi_kdv_dahil_tutar    = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN   );
    QLineEdit   * lineEdit_seri_no_list                     = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SERI_NO_LIST_COLUMN                  );

    QLineEdit   * lineEdit_stok_takip_sekli                 = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number,  STOK_TAKIP_SEKLI_COLUMN         );

    FATURA_CLEAR_FIS_SATIRI_STRUCT ( M_FIS_SATIRI );

    M_FIS_SATIRI->fis_id                            = p_fis_id;
    M_FIS_SATIRI->urun_id                           = lineEdit_urun_id->text().toInt();
    M_FIS_SATIRI->urun_adi                          = lineEdit_urun_adi->text();
    M_FIS_SATIRI->satirdaki_urun_miktari            = commaEdit_satirdaki_urun_miktari->GET_DOUBLE();
    M_FIS_SATIRI->iade_edilen_miktar                = commaEdit_satirdaki_urun_miktari->GET_DOUBLE();
    M_FIS_SATIRI->urun_birimi                       = comboBox_urun_birimleri->currentText();
    M_FIS_SATIRI->isk_oncesi_urun_birim_fiyati      = commaEdit_iskonto_oncesi_birim_fiyat->GET_DOUBLE();
    M_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati     = commaEdit_iskonto_sonrasi_birim_fiyat->GET_DOUBLE();
    M_FIS_SATIRI->satir_iskonto_yuzdesi             = commaEdit_iskonto_yuzdesi->GET_DOUBLE();
    M_FIS_SATIRI->satir_iskonto_tutari              = commaEdit_iskonto_tutari->GET_DOUBLE();
    M_FIS_SATIRI->satir_tutari                      = commaEdit_satir_tutari->GET_DOUBLE();
    M_FIS_SATIRI->doviz_id                          = DVZ_GET_DOVIZ_ID ( lineedit_para_birimi->text() );
    M_FIS_SATIRI->doviz_kodu                        = lineedit_para_birimi->text();
    M_FIS_SATIRI->satir_vergi_haric_tutar           = commaEdit_vergi_haric_tutar->GET_DOUBLE();
    M_FIS_SATIRI->kdv_orani                         = comboBox_kdv_orani->currentText().toDouble();
    M_FIS_SATIRI->satir_kdv_tutari                  = commaEdit_kdv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->satir_kdv_dahil_tutar             = commaEdit_kdv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->kdv_dahil_mi                      = comboBox_kdv_dahil_haric->currentIndex();
    M_FIS_SATIRI->order_number                      = m_ui->tableWidget_fatura_satirlari->item ( p_row_number,ORDER_COLUMN)->text().toInt();
    M_FIS_SATIRI->temel_birim_katsayisi             = commaEdit_temel_birim_katsayisi->GET_DOUBLE();
    M_FIS_SATIRI->otv_orani                         = comboBox_otv_orani->currentText().toDouble();
    M_FIS_SATIRI->satir_otv_tutari                  = commaEdit_otv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->satir_otv_dahil_tutar             = commaEdit_otv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->irsaliye_satiri_id                = lineEdit_irsaliye_satiri_id->text().toInt();
    M_FIS_SATIRI->parti_id                          = lineEdit_parti_id->text().toInt();
    M_FIS_SATIRI->parti_kodu                        = lineEdit_seri_no_list->text();
    M_FIS_SATIRI->iade_edilen_fatura_satiri_id      = lineEdit_iade_edilen_fat_satiri_id->text().toInt();
    M_FIS_SATIRI->toplu_iskonto_yuzdesi             = commaEdit_toplu_iskonto_yuzdesi->GET_DOUBLE();
    M_FIS_SATIRI->t_is_son_birim_fiyati             = commaEdit_t_is_sonrasi_birim_fiyat->GET_DOUBLE();
    M_FIS_SATIRI->t_is_son_satir_tutari             = commaEdit_t_is_sonrasi_satir_tutari->GET_DOUBLE();
    M_FIS_SATIRI->t_is_son_str_vergi_haric_tutar    = commaEdit_t_is_sonrasi_vergi_haric_tutar->GET_DOUBLE();
    M_FIS_SATIRI->t_is_son_satir_otv_tutari         = commaEdit_t_is_sonrasi_otv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->t_is_son_satir_otv_dahil_tutar    = commaEdit_t_is_sonrasi_otv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->t_is_son_satir_kdv_tutari         = commaEdit_t_is_sonrasi_kdv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->t_is_son_satir_kdv_dahil_tutar    = commaEdit_t_is_sonrasi_kdv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->modul_id                          = FATURA_MODULU;
    M_FIS_SATIRI->program_id                        = E9_PROGRAMI;

    if( lineEdit_seri_no_list->text().isEmpty() EQ false ) {
        M_FIS_SATIRI->seri_no_list                  = lineEdit_seri_no_list->text().split(",");
    }

    if ( lineEdit_stok_takip_sekli->text().toInt() EQ ENUM_SERI_NO_ILE_TAKIP ) {
        M_FIS_SATIRI->hatali_satir_mi = 0;
    }
    else if ( lineEdit_urun_id->text().toInt() > 0 ) {
        if ( commaEdit_satirdaki_urun_miktari->GET_DOUBLE() EQ 0.0 ) {
            M_FIS_SATIRI->hatali_satir_mi = 1;
        }
    }

    M_FIS_SATIRI->irsaliyelenen_miktar = 0;

    if( m_fatura_turu EQ ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS OR m_fatura_turu EQ ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS ) {
        M_FIS_SATIRI->irsaliyelenen_miktar = M_FIS_SATIRI->satirdaki_urun_miktari;
    }

    int fis_satiri_id = FATURA_FIS_SATIRI_EKLE ( M_FIS, M_FIS_SATIRI );

    // irsaliye satir ekledikten sonra olustugu icin
    FATURA_BAGLANTI_BILGISINI_GOSTER( M_FIS->fatura_turu, p_fis_id );

    QTableWidgetItem * item;

    item = new QTableWidgetItem ( QVariant ( fis_satiri_id ).toString() );
    m_ui->tableWidget_fatura_satirlari->setItem ( p_row_number, ROW_ID_COLUMN, item );

    item = new QTableWidgetItem ( QVariant ( p_fis_id ).toString() );
    m_ui->tableWidget_fatura_satirlari->setItem ( p_row_number, FIS_ID_COLUMN, item );

    M_FATURA_DETAYLARI->muh_fis_no  =  M_FIS->muh_fis_no;
    M_FATURA_DETAYLARI->cari_fis_no =  M_FIS->cari_fis_no;

    SATIR_DOVIZ_TUTARLARINI_YAZDIR      ();
    KDV_ORANLARI_BILGILERINI_YAZDIR     ();
    OTV_ORANLARI_BILGILERINI_YAZDIR     ();

    FAT_FIS_TOPLAMLARINI_YENILE( M_FIS );

    if ( ( M_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_ALIS_FATURASI OR M_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) OR
         ( M_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS OR M_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS ) ) {
        lineEdit_irsaliye_satiri_id->setText ( QVariant ( M_FIS_SATIRI->irsaliye_satiri_id ).toString() );
    }

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = FAT_GET_FATURA_TURLERI_STRING ( M_FIS->fatura_turu ) + " , " + "Fatura No : " + QVariant ( M_FIS->fis_no ).toString()
                      + " , " + "Fatura Tarihi : " + QDate::fromString( M_FIS->fatura_tarihi, "yyyy.MM.dd" ).toString("dd MMMM yyyy dddd")
                      + " , " + "Fatura : " + M_FIS->belge_seri + " " + M_FIS->belge_numarasi + "\n";

    m_log_detaylari.append ( "Cari Hesap Kodu/Adı : " + m_ui->searchEdit_cari_hesap_kodu->GET_TEXT() + "  " +
                              m_ui->lineEdit_cari_hesap_ismi->text() + " , " + FAT_GET_ACIK_KAPALI_FATURA_STRING ( M_FIS->acik_kapali_fatura ) + "\n");

    m_log_detaylari.append ( "Kdv Tutarı : "      + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_kdv_tutari->GET_DOUBLE() ).toString(), 2 ) + " , " +
                             "Fatura Tutarı : "   + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_fatura_tutari->GET_DOUBLE() ).toString(), 2 ) + "\n");

    m_log_detaylari.append ( "Ara Toplam : "        + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_ara_toplam->GET_DOUBLE() ).toString(), 2 ) + " , " +
                             "Ötv Tutarı : "        + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_tutari->GET_DOUBLE()         ).toString(), 2 ) + " , " +
                             "Ötv Dahil Tutar : "   + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->GET_DOUBLE()   ).toString(), 2 ) );


}

/**************************************************************************************
                   FAT_FATURA_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int FAT_FATURA_FISI::CHECK_UPDATE_LINE ( int p_fis_id, int p_row_number )
{
    if ( FATURADA_DEGISIKLIK_YAPILABILIR_MI ( p_fis_id ) EQ false ) {
        return ADAK_FAIL;
    }

    SQL_QUERY sql_query ( DB );

    QLineEdit   * lineEdit_urun_id                      = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN                   );
    QCommaEdit  * commaEdit_satirdaki_urun_miktari      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIRDAKI_URUN_MIKTARI_COLUMN    );
    QCommaEdit  * commaEdit_temel_birim_katsayisi       = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, TEMEL_BIRIM_KATSAYISI_COLUMN     );
    QLineEdit   * lineEdit_stok_takip_sekli             = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, STOK_TAKIP_SEKLI_COLUMN          );
    QLineEdit   * lineEdit_parti_kodu_id                = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, PARTI_ID_COLUMN                  );
    QLineEdit   * lineEdit_irsaliye_satiri_id           = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, IRSALIYE_SATIRI_ID_COLUMN        );
    QLineEdit   * lineEdit_iade_edilen_fatura_satiri_id = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, IADE_EDILEN_FAT_SATIR_ID_COLUMN  );


    int fis_satiri_id = m_ui->tableWidget_fatura_satirlari->item ( p_row_number, ROW_ID_COLUMN )->text().toInt();

    sql_query.PREPARE_SELECT("fat_fatura_satirlari ","satirdaki_urun_miktari",
                             "fis_satiri_id = :fis_satiri_id");
    sql_query.SET_VALUE ( ":fis_satiri_id", fis_satiri_id );

    if ( sql_query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr("Fatura satır bilgileri okunamadız"), NULL );
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    double      old_satirdaki_urun_miktari   = sql_query.VALUE(0).toDouble();

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
        {


            sql_query.PREPARE_SELECT("fat_irs_satir_baglantisi ","irsaliye_satiri_id ",
                                     "fatura_satiri_id = :fatura_satiri_id");
            sql_query.SET_VALUE(":fatura_satiri_id", fis_satiri_id );
            if ( sql_query.SELECT() EQ 0 ) {
                MSG_WARNING(  tr("Bağlantılı irsaliye satırı bulunamadı"), NULL );
                return ADAK_FAIL;
            }
            sql_query.NEXT();
            int old_irsaliye_satiri_id = sql_query.VALUE(0).toInt();

            STK_IRS_FIS_SATIR_STRUCT * IRS_FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;
            STK_IRS_FIS_STRUCT   * IRS_FISI       = new STK_IRS_FIS_STRUCT;

            STK_IRS_CLEAR_SATIR_STRUCT(IRS_FIS_SATIRI);
            STK_IRS_CLEAR_FIS_STRUCT  ( IRS_FISI     );

            int old_irsaliye_id = STK_GET_IRS_ID(old_irsaliye_satiri_id);

            if ( old_irsaliye_id EQ 0 ) {
                MSG_WARNING( tr("İrsaliyeden stok miktarı alınamadı"), NULL );
                return ADAK_FAIL;
            }

            STK_IRS_SATIR_BILGILERINI_OKU(old_irsaliye_satiri_id , IRS_FIS_SATIRI );

            STK_IRS_FIS_BILGILERINI_OKU( old_irsaliye_id , IRS_FISI);

            int     old_parti_kodu_id          = IRS_FIS_SATIRI->parti_id;
            int     old_urun_id                = IRS_FIS_SATIRI->urun_id;
            int     depo_id                    = IRS_FISI->depo_id;
            double  old_stok_miktari           = IRS_FIS_SATIRI->satirdaki_urun_miktari * IRS_FIS_SATIRI->temel_birim_katsayisi;
            QDate   fis_tarihi                 = QDate::fromString(IRS_FISI->fis_tarihi, "yyyy.MM.dd");

            int     new_urun_id               = lineEdit_urun_id->text().toInt();
            int     new_stok_takip_sekli      = lineEdit_stok_takip_sekli->text().toInt();
            double  new_stok_miktari          = commaEdit_satirdaki_urun_miktari->GET_DOUBLE() * commaEdit_temel_birim_katsayisi->GET_DOUBLE();
            int     new_parti_kodu_id         = lineEdit_parti_kodu_id->text().toInt();
            int     gun_no                    = MALI_YIL_ARRAY_INDIS ( fis_tarihi );

            if ( M_FIS->fatura_turu EQ ENUM_STK_STOK_ACILIS_FISI ) {
                gun_no = 0;
            }

            if ( STK_IRSALIYE_SATIR_GUNCELLEME_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU ( M_FIS->fatura_alis_satis_turu,
                                                                                      new_urun_id,
                                                                                      new_stok_takip_sekli,
                                                                                      depo_id,
                                                                                      new_stok_miktari,
                                                                                      new_parti_kodu_id,
                                                                                      old_urun_id,
                                                                                      old_stok_miktari,
                                                                                      old_parti_kodu_id,
                                                                                      gun_no,
                                                                                      this ) EQ false ) {
                return ADAK_FAIL;
            }

            break;
        }

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS  :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS :
        {

            int         new_irsaliye_satiri_id     = lineEdit_irsaliye_satiri_id->text().toInt();
            double      new_satirdaki_urun_miktari = commaEdit_satirdaki_urun_miktari->GET_DOUBLE();

            sql_query.PREPARE_SELECT("fat_irs_satir_baglantisi ","irsaliye_satiri_id",
                                     "fatura_satiri_id = :fatura_satiri_id");
            sql_query.SET_VALUE(":fatura_satiri_id", fis_satiri_id );
            if ( sql_query.SELECT() EQ 0 ) {
                QMessageBox::critical ( this, tr ( "HATA" ), tr ( "Fatura satırına başlı irsaliye satırı okunamadı" ) );
                return ADAK_FAIL;
            }
            sql_query.NEXT();
            int old_irsaliye_satiri_id = sql_query.VALUE(0).toInt();

            STK_IRS_FIS_SATIR_STRUCT * IRS_FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

            STK_IRS_CLEAR_SATIR_STRUCT(IRS_FIS_SATIRI);

            int result = STK_IRS_SATIR_BILGILERINI_OKU(old_irsaliye_satiri_id , IRS_FIS_SATIRI);

            if ( result EQ 0 ) {
                MSG_WARNING(  tr("İrsaliye satır bilgileri okunamadı"), NULL );
                return ADAK_FAIL;
            }

            sql_query.NEXT();

            double      old_irs_satirdaki_urun_miktari = IRS_FIS_SATIRI->satirdaki_urun_miktari;
            double      old_irs_faturalanan_miktar     = IRS_FIS_SATIRI->faturalanan_miktar;
            double      old_kalan_miktar               = old_irs_satirdaki_urun_miktari - old_irs_faturalanan_miktar;

            result = STK_IRS_SATIR_BILGILERINI_OKU(new_irsaliye_satiri_id , IRS_FIS_SATIRI);

            if ( old_irsaliye_satiri_id EQ new_irsaliye_satiri_id ) {
                if ( new_satirdaki_urun_miktari - old_satirdaki_urun_miktari >  old_irs_satirdaki_urun_miktari - old_irs_faturalanan_miktar ) {
                    QMessageBox::critical ( this, tr("FATURALANACAK MİKTAR AŞILDI"), tr("Seçili irsaliye satırında faturalanmamış ürün miktarı ") + QVariant( old_kalan_miktar ).toString().toUtf8() + tr(". Bu miktarı aşamazsınız.") );
                    delete IRS_FIS_SATIRI;
                    return ADAK_FAIL;
                }
            }
            else {

                if ( result EQ 0 ) {
                    MSG_WARNING(  tr("Yeni irsaliye satır bilgileri okunamadı"), NULL );
                    return ADAK_FAIL;
                }

                double      new_irs_satirdaki_urun_miktari = IRS_FIS_SATIRI->satirdaki_urun_miktari;
                double      new_irs_faturalanan_miktar     = IRS_FIS_SATIRI->faturalanan_miktar;
                double      new_kalan_miktar               = new_irs_satirdaki_urun_miktari - new_irs_faturalanan_miktar;

                if ( new_satirdaki_urun_miktari > new_kalan_miktar ) {
                    MSG_WARNING( tr("Seçili irsaliye satırında faturalanmamış ürün miktarı ") + QVariant( new_kalan_miktar ).toString().toUtf8() + tr(". Bu miktarı aşamazsınız."), NULL );
                    delete IRS_FIS_SATIRI;
                    return ADAK_FAIL;
                }
            }
            delete IRS_FIS_SATIRI;
        }

        default :
            break;
    }

    if ( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
        if ( FAT_IADE_FATURA_SATIR_GUNCELLE_ICIN_MIKTAR_KONTROLU ( lineEdit_iade_edilen_fatura_satiri_id->text().toInt(), commaEdit_satirdaki_urun_miktari->GET_DOUBLE(), this ) EQ false  ) {
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_FISI::UPDATE_LINE
***************************************************************************************/

void FAT_FATURA_FISI::UPDATE_LINE ( int p_fis_id, int p_row_number )
{

    QLineEdit   *   lineEdit_urun_id                          = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN                         );
    QLineEdit   *   lineEdit_urun_adi                         = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ADI_COLUMN                        );
    QCommaEdit  *   commaEdit_isk_oncesi_birim_fiyati         = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN          );
    QCommaEdit  *   commaEdit_isk_sonrasi_birim_fiyati        = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN         );
    QCommaEdit  *   commaEdit_iskonto_yuzdesi                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_YUZDESI_COLUMN                 );
    QCommaEdit  *   commaEdit_iskonto_tutari                  = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_TUTARI_COLUMN                  );
    QComboBox   *   comboBox_urun_birimleri                   = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN                     );
    QCommaEdit  *   commaEdit_satirdaki_urun_miktari          = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIRDAKI_URUN_MIKTARI_COLUMN          );
    QCommaEdit  *   commaEdit_satir_tutari                    = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN                    );
    QLineEdit   *   lineedit_para_birimi                      = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, FATURA_PARA_BIRIMI_COLUMN              );
    QComboBox   *   comboBox_kdv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_ORANI_COLUMN                       );
    QComboBox   *   comboBox_kdv_dahil_mi                     = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN                    );
    QCommaEdit  *   commaEdit_kdv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_TUTARI_COLUMN                      );
    QCommaEdit  *   commaEdit_kdv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_TUTAR_COLUMN                 );
    QComboBox   *   comboBox_otv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_ORANI_COLUMN                       );
    QCommaEdit  *   commaEdit_vergi_haric_tutar               = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTAR_COLUMN               );
    QCommaEdit  *   commaEdit_otv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_TUTARI_COLUMN                      );
    QCommaEdit  *   commaEdit_otv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_DAHIL_TUTAR_COLUMN                 );
    QCommaEdit  *   commaEdit_temel_birim_katsayisi           = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, TEMEL_BIRIM_KATSAYISI_COLUMN           );
    QLineEdit   *   lineEdit_parti_kodu_id                    = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, PARTI_ID_COLUMN                        );
    QLineEdit   *   lineEdit_irsaliye_satiri_id               = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, IRSALIYE_SATIRI_ID_COLUMN              );
    QLineEdit   *   lineEdit_iade_edilecek_fatura_satiri_id   = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, IADE_EDILEN_FAT_SATIR_ID_COLUMN        );
    QCommaEdit  *   commaEdit_toplu_iskonto_yuzdesi           = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, TOPLU_ISKONTO_YUZDESI_COLUMN           );
    QCommaEdit  *   commaEdit_t_is_sonrasi_birim_fiyat        = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_BIRIM_FIYAT_COLUMN        );
    QCommaEdit  *   commaEdit_t_is_sonrasi_satir_tutari       = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_SATIR_TUTARI_COLUMN       );
    QCommaEdit  *   commaEdit_t_is_sonrasi_vergi_haric_tutar  = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN  );
    QCommaEdit  *   commaEdit_t_is_sonrasi_otv_tutari         = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_OTV_TUTARI_COLUMN         );
    QCommaEdit  *   commaEdit_t_is_sonrasi_otv_dahil_tutar    = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN    );
    QCommaEdit  *   commaEdit_t_is_sonrasi_kdv_tutari         = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_KDV_TUTARI_COLUMN         );
    QCommaEdit  *   commaEdit_t_is_sonrasi_kdv_dahil_tutar    = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN    );
    QLineEdit   *   lineEdit_seri_no_list                     = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SERI_NO_LIST_COLUMN                  );

    int fis_satiri_id = m_ui->tableWidget_fatura_satirlari->item( p_row_number, ROW_ID_COLUMN )->text().toInt();


    FATURA_CLEAR_FIS_SATIRI_STRUCT ( M_FIS_SATIRI );

    M_FIS_SATIRI->urun_id                           = lineEdit_urun_id->text().toInt();
    M_FIS_SATIRI->urun_adi                          = lineEdit_urun_adi->text();
    M_FIS_SATIRI->urun_birimi                       = comboBox_urun_birimleri->currentText();
    M_FIS_SATIRI->isk_oncesi_urun_birim_fiyati      = commaEdit_isk_oncesi_birim_fiyati->GET_DOUBLE();
    M_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati     = commaEdit_isk_sonrasi_birim_fiyati->GET_DOUBLE();
    M_FIS_SATIRI->satir_iskonto_yuzdesi             = commaEdit_iskonto_yuzdesi->GET_DOUBLE();
    M_FIS_SATIRI->satir_iskonto_tutari              = commaEdit_iskonto_tutari->GET_DOUBLE();
    M_FIS_SATIRI->satir_tutari                      = commaEdit_satir_tutari->GET_DOUBLE();
    M_FIS_SATIRI->doviz_id                          = DVZ_GET_DOVIZ_ID ( lineedit_para_birimi->text());
    M_FIS_SATIRI->satir_vergi_haric_tutar           = commaEdit_vergi_haric_tutar->GET_DOUBLE();
    M_FIS_SATIRI->satir_kdv_dahil_tutar             = commaEdit_kdv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->satir_kdv_tutari                  = commaEdit_kdv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->kdv_orani                         = comboBox_kdv_orani->currentText().toDouble();
    M_FIS_SATIRI->kdv_dahil_mi                      = comboBox_kdv_dahil_mi->currentIndex();
    M_FIS_SATIRI->otv_orani                         = comboBox_otv_orani->currentText().toDouble();
    M_FIS_SATIRI->satir_otv_tutari                  = commaEdit_otv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->satir_otv_dahil_tutar             = commaEdit_otv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->temel_birim_katsayisi             = commaEdit_temel_birim_katsayisi->GET_DOUBLE();
    M_FIS_SATIRI->satirdaki_urun_miktari            = commaEdit_satirdaki_urun_miktari->GET_DOUBLE();
    M_FIS_SATIRI->parti_kodu                        = lineEdit_seri_no_list->text();
    M_FIS_SATIRI->parti_id                          = lineEdit_parti_kodu_id->text().toInt();
    M_FIS_SATIRI->irsaliyelenen_miktar              = 0.00;
    M_FIS_SATIRI->irsaliye_satiri_id                = lineEdit_irsaliye_satiri_id->text().toInt();
    M_FIS_SATIRI->irsaliye_satiri_id                = 0;
    M_FIS_SATIRI->hatali_satir_mi                   = 0;
    M_FIS_SATIRI->iade_edilen_fatura_satiri_id      = lineEdit_iade_edilecek_fatura_satiri_id->text().toInt();
    M_FIS_SATIRI->toplu_iskonto_yuzdesi             = commaEdit_toplu_iskonto_yuzdesi->GET_DOUBLE();
    M_FIS_SATIRI->t_is_son_birim_fiyati             = commaEdit_t_is_sonrasi_birim_fiyat->GET_DOUBLE();
    M_FIS_SATIRI->t_is_son_satir_tutari             = commaEdit_t_is_sonrasi_satir_tutari->GET_DOUBLE();
    M_FIS_SATIRI->t_is_son_str_vergi_haric_tutar    = commaEdit_t_is_sonrasi_vergi_haric_tutar->GET_DOUBLE();
    M_FIS_SATIRI->t_is_son_satir_otv_tutari         = commaEdit_t_is_sonrasi_otv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->t_is_son_satir_otv_dahil_tutar    = commaEdit_t_is_sonrasi_otv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->t_is_son_satir_kdv_tutari         = commaEdit_t_is_sonrasi_kdv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->t_is_son_satir_kdv_dahil_tutar    = commaEdit_t_is_sonrasi_kdv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->modul_id                          = FATURA_MODULU;
    M_FIS_SATIRI->program_id                        = E9_PROGRAMI;
    M_FIS_SATIRI->fis_id                            = p_fis_id;

    if( lineEdit_seri_no_list->text().isEmpty() EQ false ) {
        M_FIS_SATIRI->seri_no_list = lineEdit_seri_no_list->text().split(",");
    }

    FATURA_FIS_SATIRI_GUNCELLE    ( M_FIS, M_FIS_SATIRI, fis_satiri_id );

    M_FATURA_DETAYLARI->muh_fis_no  =  M_FIS->muh_fis_no;
    M_FATURA_DETAYLARI->cari_fis_no =  M_FIS->cari_fis_no;

    SATIR_DOVIZ_TUTARLARINI_YAZDIR  ();
    KDV_ORANLARI_BILGILERINI_YAZDIR ();
    OTV_ORANLARI_BILGILERINI_YAZDIR ();

    FAT_FIS_TOPLAMLARINI_YENILE( M_FIS );

    // Satir bilgilerini guncellemek icin
    SELECTED_LINE(p_row_number, -1);

    lineEdit_irsaliye_satiri_id->setText ( QVariant ( M_FIS_SATIRI->irsaliye_satiri_id ).toString() );

    QString cari_hesap_kodu;
    QString cari_hesap_ismi;

    if ( M_FIS->cari_hesap_id > 0 ) {
        CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( M_FIS->cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );
    }

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = FAT_GET_FATURA_TURLERI_STRING(M_FIS->fatura_turu) + " , " + "Fatura No : " + QVariant ( M_FIS->fis_no ).toString() + " , " +
                      "Fatura Tarihi : " + QDate::fromString ( M_FIS->fatura_tarihi, "yyyy.MM.dd" ).toString ( "dd MMMM yyyy dddd" ) + " , " + "Fatura : " +
                      M_FIS->belge_seri + " "  + M_FIS->belge_numarasi + "\n";

    m_log_detaylari.append ( "Cari Hesap Kodu/Adı : " + cari_hesap_kodu + "  " +
                              cari_hesap_ismi + " , " + FAT_GET_ACIK_KAPALI_FATURA_STRING ( M_FIS->acik_kapali_fatura ) + "\n" );

    m_log_detaylari.append ( "Kdv Tutarı : "      + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_kdv_tutari->GET_DOUBLE() ).toString(), 2 ) + " , " +
                             "Fatura Tutarı : "   + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_fatura_tutari->GET_DOUBLE() ).toString(), 2 ) + "\n" );

    m_log_detaylari.append ( "Ara Toplam : "        + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_ara_toplam->GET_DOUBLE() ).toString(), 2 ) + " , " +
                             "Ötv Tutarı : "        + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_tutari->GET_DOUBLE()         ).toString(), 2 ) + " , " +
                             "Ötv Dahil Tutar : "   + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->GET_DOUBLE()   ).toString(), 2 ) );
}

/**************************************************************************************
                   FAT_FATURA_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int FAT_FATURA_FISI::CHECK_DELETE_LINE ( int p_fis_id, int p_row_number )
{
    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS           :
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS          :

            MSG_WARNING(  tr("Otomatik irsaliye faturalaştırma yaparken, elle satır silemezsiniz."), NULL );
            return ADAK_FAIL;
        default :
            break;
    }
    SQL_QUERY sql_query ( DB );

    int fis_satiri_id = m_ui->tableWidget_fatura_satirlari->item ( p_row_number, ROW_ID_COLUMN )->text().toInt();


    if ( FATURADA_DEGISIKLIK_YAPILABILIR_MI ( p_fis_id ) EQ false ) {
        return ADAK_FAIL;
    }

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
        {
            sql_query.PREPARE_SELECT("fat_irs_satir_baglantisi ","irsaliye_satiri_id",
                                     "fatura_satiri_id = :fatura_satiri_id");
            sql_query.SET_VALUE ( ":fatura_satiri_id", fis_satiri_id );

            if ( sql_query.SELECT() EQ 0 ) {
                MSG_WARNING(  tr("Fatura satırı okunamadı."), NULL );
                return ADAK_FAIL;
            }

            sql_query.NEXT();

            int irsaliye_satiri_id = sql_query.VALUE(0).toInt();      

            if ( STK_IRSALIYE_SATIR_SILME_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU ( irsaliye_satiri_id, M_FIS->fatura_alis_satis_turu, this ) EQ false ) {
                return ADAK_FAIL;
            }

            // irsaliye yazdirilmis ise hicbir sekilde fatura uzerinde degisiklik yapilamaz..
            int irsaliye_id = STK_GET_FAT_IRS_BAGLANTISI_IRSALIYE_ID( p_fis_id );

            if ( IRS_GET_IRSALIYE_YAZDIRILDIMI( irsaliye_id ) EQ true ) {
                MSG_WARNING( tr( "İrsaliye Yazdırıldığı için fiş üzerinde değişiklik yapamazsınız.!"), NULL );
                return ADAK_FAIL;
            }

            break;
        }
        default :
            break;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_FISI::DELETE_LINE
***************************************************************************************/

void FAT_FATURA_FISI::DELETE_LINE ( int p_fis_id, int p_row_number )
{
    Q_UNUSED( p_fis_id );

    int fis_satiri_id = m_ui->tableWidget_fatura_satirlari->item( p_row_number, ROW_ID_COLUMN )->text().toInt();

    FATURA_FIS_SATIRI_STRUCT FIS_SATIRI;

    FATURA_CLEAR_FIS_SATIRI_STRUCT( &FIS_SATIRI);

    FATURA_SATIR_BILGILERINI_OKU( &FIS_SATIRI, fis_satiri_id );

    FATURA_FIS_SATIRI_SIL ( M_FIS, &FIS_SATIRI, fis_satiri_id );

    FAT_FIS_TOPLAMLARINI_YENILE( M_FIS );

    M_FATURA_DETAYLARI->muh_fis_no  =  M_FIS->muh_fis_no;
    M_FATURA_DETAYLARI->cari_fis_no =  M_FIS->cari_fis_no;

    SATIR_DOVIZ_TUTARLARINI_YAZDIR  ();
    KDV_ORANLARI_BILGILERINI_YAZDIR ();
    OTV_ORANLARI_BILGILERINI_YAZDIR ();

    QString cari_hesap_kodu, cari_hesap_ismi;

    if ( M_FIS->cari_hesap_id > 0 ) {
        CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( M_FIS->cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );
    }

    if ( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
        //! silinen + bos satir = 2
        if ( m_ui->tableWidget_fatura_satirlari->rowCount() EQ 2 ) {
            m_ui->button_fatura_secimi->setEnabled( true );
        }
    }

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = FAT_GET_FATURA_TURLERI_STRING ( M_FIS->fatura_turu ) + " , " + "Fatura No : " + QVariant ( M_FIS->fis_no ).toString() + " , " +
                      "Fatura Tarihi : " + QDate::fromString ( M_FIS->fatura_tarihi, "yyyy.MM.dd" ).toString ( "dd MMMM yyyy dddd" ) + " , " +
                      "Fatura : " + M_FIS->belge_seri + " " + M_FIS->belge_numarasi + "\n";

    m_log_detaylari.append ( "Cari Hesap Kodu/Adı : " + cari_hesap_kodu + "  " +
                              cari_hesap_ismi + " , " + FAT_GET_ACIK_KAPALI_FATURA_STRING ( M_FIS->acik_kapali_fatura ) + "\n" );

    m_log_detaylari.append ( "Kdv Tutarı : "      + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_kdv_tutari->GET_DOUBLE() ).toString(), 2 ) + " , " +
                             "Fatura Tutarı : " + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_fatura_tutari->GET_DOUBLE() ).toString(), 2 ) + "\n" );

    m_log_detaylari.append ( "Ara Toplam : "        + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_ara_toplam->GET_DOUBLE()   ).toString(), 2 ) + " , " +
                             "Ötv Tutarı : "        + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_tutari->GET_DOUBLE() ).toString(), 2 ) + " , " +
                             "Ötv Dahil Tutar : "   + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->GET_DOUBLE() ).toString(), 2 ));

}

/**************************************************************************************
                   FAT_FATURA_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int FAT_FATURA_FISI::LOCK_FIS_RECORD ( int p_fis_id )
{
    return DB->LOCK_ROW( "fis_id","fat_faturalar", QString ( "fis_id = %1" ).arg ( p_fis_id ));
}

/**************************************************************************************
                   FAT_FATURA_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void FAT_FATURA_FISI::UNLOCK_FIS_RECORD ( int p_fis_id )
{
    DB->UNLOCK_ROW( "fis_id","fat_faturalar", QString ( "fis_id = %1" ).arg ( p_fis_id ));
}

/**************************************************************************************
                   FAT_FATURA_FISI::SEARCH_EDIT_CLICKED
***************************************************************************************/

void FAT_FATURA_FISI::SEARCH_EDIT_CLICKED(QWidget * widget, QLineEdit * line_edit)
{

    if ( widget EQ m_ui->searchEdit_cari_hesap_kodu ) {
        int cari_hesap_id = OPEN_CARI_KART_ARAMA( line_edit->text(), SAHIS_ARTI_FIRMA,this,true , E9_PROGRAMI, CARI_MODULU );

        if ( cari_hesap_id NE -1 ) {
            line_edit->setText(CARI_FIND_HESAP_KODU(cari_hesap_id));
        }
        M_FIS->cari_hesap_id = cari_hesap_id;
    }
    else if ( widget EQ m_ui->searchEdit_depo_kodu ) {

        int depo_id = SUBE_DEPO_SEC ( SUBE_GET_SUBE_ID(), this );

        if ( depo_id NE -1 ) {
            line_edit->setText ( SUBE_GET_DEPO_KODU ( depo_id ) );
        }
    }
}
/**************************************************************************************
                   FAT_FATURA_FISI::LINE_SEARCH_EDIT_CLICKED
***************************************************************************************/

void FAT_FATURA_FISI::LINE_SEARCH_EDIT_CLICKED ( int p_row_number, QWidget * p_widget, QLineEdit * p_lineEdit  )
{
    Q_UNUSED ( p_lineEdit );

    QSearchEdit * searchedit_urun_kodu           = ( QSearchEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_KODU_COLUMN            );
    QLineEdit   * lineedit_urun_id               = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN              );
    QLineEdit   * lineEdit_stok_takip_sekli      = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number,  STOK_TAKIP_SEKLI_COLUMN    );

    M_FIS_SATIRI->urun_id = lineedit_urun_id->text().toInt();

    if ( p_widget EQ searchedit_urun_kodu ) {

        int urun_id = -1;
        if ( M_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI  OR M_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS) {
            urun_id = OPEN_STOK_URUN_ARAMA ( ENUM_URUN_HIZMET_KARTI, M_FIS->depo_id, searchedit_urun_kodu->GET_TEXT(),-1, this );
            lineEdit_stok_takip_sekli->setText( QVariant ( STK_GET_URUN_TAKIP_SEKLI( urun_id )).toString());
        }
        else if ( M_FIS->fatura_turu EQ ENUM_FAT_IRSALIYELI_ALIS_FATURASI  OR  M_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS) {
            urun_id = OPEN_STOK_URUN_ARAMA ( ENUM_URUN_HIZMET_KARTI , -1 ,searchedit_urun_kodu->GET_TEXT(),-1,this);
            lineEdit_stok_takip_sekli->setText( QVariant ( STK_GET_URUN_TAKIP_SEKLI( urun_id )).toString());
        }
        else if ( M_FIS->fatura_turu EQ ENUM_FAT_HIZMET_ALIS_FATURASI ) {
            urun_id = OPEN_STOK_URUN_ARAMA ( ENUM_HIZMET_KARTI , -1 ,searchedit_urun_kodu->GET_TEXT(), ENUM_ALINAN_HIZMET,this);
        }
        else if ( M_FIS->fatura_turu EQ ENUM_FAT_HIZMET_SATIS_FATURASI ) {
            urun_id = OPEN_STOK_URUN_ARAMA ( ENUM_HIZMET_KARTI , -1 ,searchedit_urun_kodu->GET_TEXT(), ENUM_SATILAN_HIZMET,this);
        }
        else {
            urun_id = OPEN_STOK_URUN_ARAMA ( ENUM_URUN_HIZMET_KARTI, M_FIS->depo_id, searchedit_urun_kodu->GET_TEXT(),-1, this );
        }
        if ( urun_id NE -1 ) {
            // depo kontrolu ilk defa urun eklendiginde once bir alis islemi yapmali yoksa arraylar olusturulmuyor.
            if ( M_FIS->fatura_alis_satis_turu EQ ENUM_SATIS_IRSALIYESI_FATURASI ) {
                switch( M_FIS->fatura_turu ) {
                    case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
                    case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
                    case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
                    case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS : {

                        SQL_QUERY query( DB );

                        if ( STK_GET_HIZMET_MI( urun_id ) EQ HAYIR ) {
                            query.PREPARE_SELECT( "stk_depodaki_urunler", "kayit_id", "depo_id = :depo_id AND urun_id = :urun_id " );
                            query.SET_VALUE     ( ":depo_id", M_FIS->depo_id );
                            query.SET_VALUE     ( ":urun_id", urun_id );

                            if( query.SELECT() EQ 0 ) {
                                MSG_WARNING( tr("Bu Üründen depoda bulunmamaktadır.!"), searchedit_urun_kodu );
                                return;
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            STK_URUNLER_STRUCT URUN;

            STK_URUNLER_TABLOSUNU_OKU( urun_id, &URUN );

            searchedit_urun_kodu->SET_TEXT( URUN.urun_kodu );
        }
   }

}

/**************************************************************************************
                   FAT_FATURA_FISI::SELECTED_LINE
***************************************************************************************/

void FAT_FATURA_FISI::SELECTED_LINE ( int p_row_number, int p_row_save_status )
{
    Q_UNUSED ( p_row_save_status );

    //QCommaEdit   * commaEdit_iskonto_yuzdesi                = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_YUZDESI_COLUMN           );
    //QCommaEdit   * commaEdit_iskonto_tutari                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_TUTARI_COLUMN            );
    QCommaEdit   * commaEdit_satirdaki_urun_miktari         = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIRDAKI_URUN_MIKTARI_COLUMN    );
    QCommaEdit   * commaEdit_kdv_tutari                     = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_TUTARI_COLUMN                );
    QCommaEdit   * commaEdit_kdv_dahil_tutar                = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_TUTAR_COLUMN           );
    QComboBox    * comboBox_kdv_dahil_mi                    = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN              );
    QCommaEdit   * commaEdit_vergi_haric_tutar              = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTAR_COLUMN         );
    QCommaEdit   * commaEdit_otv_tutari                     = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_TUTARI_COLUMN                );
    QCommaEdit   * commaEdit_otv_dahil_tutar                = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_DAHIL_TUTAR_COLUMN           );
    QLineEdit    * lineEdit_seri_no_list                    = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SERI_NO_LIST_COLUMN              );

    QString satir_tutar_bilgileri;

    double satirdaki_urun_miktari                  = commaEdit_satirdaki_urun_miktari->GET_DOUBLE();
    double vergi_haric_tutar                       = commaEdit_vergi_haric_tutar->GET_DOUBLE();
    double kdv_tutari                              = commaEdit_kdv_tutari->GET_DOUBLE();
    double kdv_dahil_tutar                         = commaEdit_kdv_dahil_tutar->GET_DOUBLE();
    double otv_tutari                              = commaEdit_otv_tutari->GET_DOUBLE();
    double otv_dahil_tutar                         = commaEdit_otv_dahil_tutar->GET_DOUBLE();
    double makbuz_para_birimi_cinsinden_birim_fiyat = 0.00;

    if ( comboBox_kdv_dahil_mi->currentIndex() EQ 0 ) {
        makbuz_para_birimi_cinsinden_birim_fiyat = commaEdit_vergi_haric_tutar->GET_DOUBLE();
        satir_tutar_bilgileri                  += tr ( "KDV Dahil Birim Fiyat : " ) +  VIRGUL_EKLE ( QVariant ( ROUND( makbuz_para_birimi_cinsinden_birim_fiyat / satirdaki_urun_miktari )  ).toString() ) + " " + M_FIS->fatura_para_birimi_kodu + "\n" ;
    }
    else {
        makbuz_para_birimi_cinsinden_birim_fiyat = commaEdit_vergi_haric_tutar->GET_DOUBLE();
        satir_tutar_bilgileri                  += tr ( "KDV Hariç Birim Fiyat : " ) + VIRGUL_EKLE( QVariant ( ROUND( makbuz_para_birimi_cinsinden_birim_fiyat / satirdaki_urun_miktari ) ).toString() ) + " " + M_FIS->fatura_para_birimi_kodu + "\n" ;
    }

    satir_tutar_bilgileri += tr ( "Satır Vergi Hariç Tutar : ")  + VIRGUL_EKLE ( QVariant ( vergi_haric_tutar ).toString() ) + " " + M_FIS->fatura_para_birimi_kodu + "\n";
    satir_tutar_bilgileri += tr ( "Satır ÖTV Tutarı        : ")  + VIRGUL_EKLE ( QVariant ( otv_tutari ).toString() )        + " " + M_FIS->fatura_para_birimi_kodu + "\n";
    satir_tutar_bilgileri += tr ( "Satır ÖTV Dahil Tutar   : ")  + VIRGUL_EKLE ( QVariant ( otv_dahil_tutar ).toString() )   + " " + M_FIS->fatura_para_birimi_kodu + "\n";
    satir_tutar_bilgileri += tr ( "Satır KDV Tutarı        : ")  + VIRGUL_EKLE ( QVariant ( kdv_tutari ).toString() )        + " " + M_FIS->fatura_para_birimi_kodu + "\n";
    satir_tutar_bilgileri += tr ( "Satır KDV Dahil Tutar   : ")  + VIRGUL_EKLE ( QVariant ( kdv_dahil_tutar ).toString() )   + " " + M_FIS->fatura_para_birimi_kodu + "\n";

    m_ui->textEdit_satir_bilgileri->setText ( satir_tutar_bilgileri );

    double satir_iskonto_tutari = 0.00;

    QCommaEdit   * satir_iskonto_yuzdesi                = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_YUZDESI_COLUMN           );
    QCommaEdit   * satir_urun_miktari                   = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIRDAKI_URUN_MIKTARI_COLUMN    );
    QCommaEdit   * satir_iskonto_sonrasi_birim_fiyat    = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN   );
    satir_iskonto_tutari = ROUND( satir_iskonto_sonrasi_birim_fiyat->GET_DOUBLE() * satir_urun_miktari->GET_DOUBLE() * ( satir_iskonto_yuzdesi->GET_DOUBLE() / 100.0 ) );

    QString iskonto_tutar_bilgileri = "";
    iskonto_tutar_bilgileri += "Satir İskonto Tutarı : " +  VIRGUL_EKLE( QVariant ( satir_iskonto_tutari ).toString() ) + " " + M_FIS->fatura_para_birimi_kodu + "\n";
    m_ui->textEdit_iskonto_tutarlari->setText ( iskonto_tutar_bilgileri );

    m_ui->textEdit_seri_no_or_parti_kodu->setText( lineEdit_seri_no_list->text().replace(",","\n"));

}

/**************************************************************************************
                   FAT_FATURA_FISI::LEAVE_RECORD
***************************************************************************************/

int FAT_FATURA_FISI::LEAVE_RECORD()
{
    if ( m_log_kaydi_eklenecek_mi EQ true ) {

        DB->START_TRANSACTION();

        if ( m_kayit_eklendi EQ true ) {
            E9_LOG_KAYDI_EKLE (FATURA_MODULU , LOG_FAT_FATURALAR , LOG_ISLEM_ADD , m_log_detaylari);
        }
        if ( m_kayit_silindi EQ true ) {
            E9_LOG_KAYDI_EKLE (FATURA_MODULU , LOG_FAT_FATURALAR , LOG_ISLEM_DELETE , m_log_detaylari);
        }
        if ( m_kayit_eklendi EQ false AND m_kayit_silindi EQ false ) {
            E9_LOG_KAYDI_EKLE (FATURA_MODULU , LOG_FAT_FATURALAR , LOG_ISLEM_UPDATE , m_log_detaylari);
        }

        DB->COMMIT_TRANSACTION();
    }
    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_FISI::URUN_BILGILERINI_AL
***************************************************************************************/

void FAT_FATURA_FISI::URUN_BILGILERINI_AL ( int p_row_number )
{
    QLineEdit  * lineEdit_urun_id = ( QLineEdit  *  ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN );

    int urun_id = lineEdit_urun_id->text().toInt();

    if ( urun_id < 1 ) {
        return;
    }

    if ( m_last_urun_id NE urun_id ) {

        m_last_urun_id = urun_id;

        STK_URUNLER_STRUCT * URUN = new STK_URUNLER_STRUCT;

        STK_URUNLER_TABLOSUNU_OKU(urun_id , URUN , true);

        UNPACK_DOUBLE_ARRAY(PACK_DOUBLE_ARRAY(URUN->urun_birim_katsayilari_array  ,URUN_BIRIMLERI_SAYISI) ,M_STOK_BIRIM_KATSAYILARI    ,URUN_BIRIMLERI_SAYISI);
        UNPACK_INT_ARRAY   (PACK_INT_ARRAY(URUN->urun_capraz_birimler_array       ,URUN_BIRIMLERI_SAYISI)  ,M_STOK_CAPRAZ_BIRIMLER      ,URUN_BIRIMLERI_SAYISI);
        UNPACK_DOUBLE_ARRAY(PACK_DOUBLE_ARRAY(URUN->urun_birim_alis_fiyatlar_array,URUN_BIRIMLERI_SAYISI),M_STOK_BIRIM_ALIS_FIYATLARI ,URUN_BIRIMLERI_SAYISI);
        UNPACK_DOUBLE_ARRAY(PACK_DOUBLE_ARRAY(URUN->urun_birim_sts_fiyatlar_array ,URUN_BIRIMLERI_SAYISI) ,M_STOK_BIRIM_SATIS_FIYATLARI,URUN_BIRIMLERI_SAYISI);

        delete URUN;
    }

}

/**************************************************************************************
                   FAT_FATURA_FISI::TEMEL_BIRIM_CINSINDEN_KATSAYI_HESAPLA
***************************************************************************************/

void FAT_FATURA_FISI::TEMEL_BIRIM_CINSINDEN_KATSAYI_HESAPLA ( int p_row_number )
{
    QLineEdit  * lineedit_urun_id                = ( QLineEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN);
    QComboBox  * combobox_urun_birimi            = ( QComboBox  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN);
    QCommaEdit * commaedit_temel_birim_katsayisi = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, TEMEL_BIRIM_KATSAYISI_COLUMN);

    int urun_id = lineedit_urun_id->text().toInt();

    if ( urun_id < 1 ) {
        return;
    }

    double katsayi = 1.0;

    for ( int i = 0 ; i < URUN_BIRIMLERI_SAYISI ; i++ ) {
        if ( M_STOK_BIRIM_ADLARI[i] NE combobox_urun_birimi->currentText() ) {
            continue;
        }
        katsayi          = 1.0;
        int capraz_birim = M_STOK_CAPRAZ_BIRIMLER   [i];
        katsayi          = ROUND ( M_STOK_BIRIM_KATSAYILARI [i] * katsayi  );

        while ( capraz_birim > 0 ) {
            katsayi       = ROUND ( katsayi * M_STOK_BIRIM_KATSAYILARI [ capraz_birim - 1 ] );
            capraz_birim  = M_STOK_CAPRAZ_BIRIMLER   [ capraz_birim - 1 ];
        }
        break;
    }

    commaedit_temel_birim_katsayisi->SET_DOUBLE ( katsayi );

}

/**************************************************************************************
                   FAT_FATURA_FISI::URUN_BIRIMI_FIYATLARINI_HESAPLA
***************************************************************************************/

void FAT_FATURA_FISI::URUN_BIRIMI_FIYATLARINI_HESAPLA ( int p_row_number )
{
    QComboBox  * comboBox_stok_birimi               = ( QComboBox  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN                );
    QCommaEdit * commaEdit_isk_oncesi_birim_fiyat   = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN     );
    QCommaEdit * commaEdit_isk_sonrasi_birim_fiyat  = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN    );
    QCommaEdit * commaEdit_iskonto_yuzdesi          = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_YUZDESI_COLUMN            );
    QCommaEdit * commaEdit_iskonto_tutari           = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_TUTARI_COLUMN             );
    QCommaEdit * commaEdit_satir_tutari             = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN               );
    QCommaEdit * commaEdit_satirdaki_urun_miktari   = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIRDAKI_URUN_MIKTARI_COLUMN     );
    QLineEdit  * lineEdit_urun_id                   = ( QLineEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN                    );
    QComboBox  * comboBox_kdv_orani                 = ( QComboBox  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_ORANI_COLUMN                  );
    QComboBox  * comboBox_otv_orani                 = ( QComboBox  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_ORANI_COLUMN                  );
    QComboBox  * comboBox_kdv_dahil_mi              = ( QComboBox  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN               );

    double  satir_tutari = 0.00;

    commaEdit_iskonto_yuzdesi->clear();
    commaEdit_iskonto_tutari->clear();

    if ( comboBox_stok_birimi->currentIndex() EQ 0 ) {

        STK_URUNLER_STRUCT * URUN = new STK_URUNLER_STRUCT;
        STK_URUNLER_TABLOSUNU_OKU(lineEdit_urun_id->text().toInt() , URUN);

        // Alış ve satış fiyatı fiş/irsaliye türüne göre seçilecek

        switch ( M_FIS->fatura_alis_satis_turu ) {

            case     ENUM_ALIS_IRSALIYESI_FATURASI :
        default                                : {
                if ( M_FIS->fatura_para_birimi_id EQ URUN->urun_alis_doviz_id ) {
                    commaEdit_isk_oncesi_birim_fiyat->SET_DOUBLE ( URUN->alis_fiyati );
                    commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE ( URUN->alis_fiyati );
                }
                else {
                    double urun_fiyati = DVZ_PARA_BIRIMLERINI_DONUSTUR ( URUN->urun_alis_doviz_id, URUN->alis_fiyati, M_FIS->fatura_para_birimi_id , m_ui->adakDate_fatura_tarihi->QDATE(), ENUM_ALIS );
                    commaEdit_isk_oncesi_birim_fiyat->SET_DOUBLE ( urun_fiyati );
                    commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE ( urun_fiyati );
                }
                break;

            }
            case ENUM_SATIS_IRSALIYESI_FATURASI : {
                if ( M_FIS->fatura_para_birimi_id EQ URUN->urun_satis_doviz_id )  {
                    commaEdit_isk_oncesi_birim_fiyat->SET_DOUBLE ( URUN->satis_fiyati );
                    commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE ( URUN->satis_fiyati );
                }
                else {
                    double urun_fiyati = DVZ_PARA_BIRIMLERINI_DONUSTUR ( URUN->urun_satis_doviz_id, URUN->satis_fiyati, M_FIS->fatura_para_birimi_id , m_ui->adakDate_fatura_tarihi->QDATE(), ENUM_SATIS );
                    commaEdit_isk_oncesi_birim_fiyat->SET_DOUBLE ( urun_fiyati );
                    commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE ( urun_fiyati );
                }
                break;
            }

        }
        satir_tutari = ROUND ( commaEdit_satirdaki_urun_miktari->GET_DOUBLE() * commaEdit_isk_sonrasi_birim_fiyat->GET_DOUBLE() );
    }

    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
        if ( M_STOK_BIRIM_ADLARI[i] NE comboBox_stok_birimi->currentText() ) {
            continue;
        }
        commaEdit_isk_oncesi_birim_fiyat->SET_DOUBLE  ( M_STOK_BIRIM_ALIS_FIYATLARI[i] );
        commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE ( M_STOK_BIRIM_ALIS_FIYATLARI[i] );
        satir_tutari = ROUND ( M_STOK_BIRIM_ALIS_FIYATLARI[i] * commaEdit_satirdaki_urun_miktari->GET_DOUBLE()  );
        break;
    }

    double kdv_orani          = comboBox_kdv_orani->currentText().toDouble();
    double otv_orani          = comboBox_otv_orani->currentText().toDouble();
    double vergi_haric_tutar  = 0.00;


    if ( comboBox_kdv_dahil_mi->currentIndex() EQ 1 ) {
        vergi_haric_tutar = ROUND ( satir_tutari / ( 1 + ( kdv_orani / 100 ) ) );
        vergi_haric_tutar = ROUND ( vergi_haric_tutar / ( 1 + ( otv_orani / 100 ) ) );
    }
    else {
        vergi_haric_tutar = satir_tutari;
    }
    commaEdit_satir_tutari->SET_DOUBLE ( vergi_haric_tutar );
}


/**************************************************************************************
                   FAT_FATURA_FISI::SATIR_DOVIZ_TUTARLARINI_YAZDIR
***************************************************************************************/

void FAT_FATURA_FISI::SATIR_DOVIZ_TUTARLARINI_YAZDIR ()
{
    QStringList SATIR_BILGILERI;
    QString     print_text;

    m_ui->textEdit_doviz_bilgileri->clear();

    for ( int i = 0; i < M_FIS->DOVIZ_CINSINDEN_SATIR_TOPLAMLARI.size(); i++ ) {
            SATIR_BILGILERI = M_FIS->DOVIZ_CINSINDEN_SATIR_TOPLAMLARI[i].split("\t");
            print_text   += VIRGUL_EKLE ( QVariant ( ROUND( SATIR_BILGILERI.at(0).toDouble() )).toString() ).rightJustified(8) + " " + SATIR_BILGILERI.at(1) + "\n" ;
    }
    m_ui->textEdit_doviz_bilgileri->setText(print_text);
}


/**************************************************************************************
                   FAT_FATURA_FISI::KDV_ORANLARI_BILGILERINI_YAZDIR
***************************************************************************************/

void FAT_FATURA_FISI::KDV_ORANLARI_BILGILERINI_YAZDIR ()
{
    QString     print_text;

    m_ui->textEdit_kdv_tutarlari->clear();

    for ( int i = 0; i < M_FIS->kdv_orani_sayisi; i++ ) {
        if ( M_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY[i] EQ 0 OR M_FIS->KDV_ORANLARI_ARRAY[i] EQ 0 ) {
            continue;
        }
        print_text   += "% " + QVariant ( M_FIS->KDV_ORANLARI_ARRAY[i] ).toString() + "  : " + VIRGUL_EKLE ( QVariant ( M_FIS->KDV_ORANINA_GORE_TUTARLAR_ARRAY[i] ).toString() , 2 ).rightJustified(8) + " " + M_FIS->fatura_para_birimi_kodu + "\n" ;
    }
    m_ui->textEdit_kdv_tutarlari->setText ( print_text );
}


/**************************************************************************************
                   FAT_FATURA_FISI::OTV_ORANLARI_BILGILERINI_YAZDIR
***************************************************************************************/

void FAT_FATURA_FISI::OTV_ORANLARI_BILGILERINI_YAZDIR ()
{
    QString     print_text;
    m_ui->textEdit_otv_tutarlari->clear();

    for ( int i = 0; i < M_FIS->otv_orani_sayisi; i++ ) {
        if ( M_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY[i] EQ 0 OR M_FIS->OTV_ORANLARI_ARRAY[i] EQ 0 ) {
            continue;
        }
        print_text   += "% " + QVariant ( M_FIS->OTV_ORANLARI_ARRAY[i] ).toString() + " : " + VIRGUL_EKLE ( QVariant ( M_FIS->OTV_ORANINA_GORE_TUTARLAR_ARRAY[i] ).toString() , 2 ).rightJustified(8) + " " + M_FIS->fatura_para_birimi_kodu + "\n" ;
    }
    m_ui->textEdit_otv_tutarlari->setText ( print_text );
}


/**************************************************************************************
                   FAT_FATURA_FISI::FATURADA_DEGISIKLIK_YAPILABILIR_MI
***************************************************************************************/

bool FAT_FATURA_FISI::FATURADA_DEGISIKLIK_YAPILABILIR_MI ( int p_fis_id, int p_delete_record_check )
{
    SQL_QUERY query ( DB );

    int     iade_edilen_satir_iceriyor_mu   =   0;

    query.PREPARE_SELECT ( "fat_fatura_satirlari ","fis_satiri_id",
                           "fis_id = :fis_id AND iade_edilen_miktar > :iade_edilen_miktar");
    query.SET_VALUE      ( ":fis_id"             , p_fis_id );
    query.SET_VALUE      ( ":iade_edilen_miktar" , 0);

    if ( query.SELECT() > 0 ) {
        iade_edilen_satir_iceriyor_mu = 1;
    }

    if ( iade_edilen_satir_iceriyor_mu EQ 1 ) {
        QMessageBox::critical ( this, tr ("HATA"), tr("Bu fatura iade içeriyor. Silinemez veya değiştirilemez.") );
        return false;
    }

    if ( M_FIS->iptal_mi EQ 1 ) {
        MSG_WARNING(  tr("İptal edilmiş fatura üzerinde değişiklik yapamazsınız."), NULL );
        return false;
    }

    if ( p_delete_record_check EQ 0 ) {     // Check fis record delete icerisinden cagirilirsa hata dondurulmez. irsaliye iptale dusurulur.
        if ( M_FIS->yazdirildi_mi EQ 1 ) {
            QMessageBox::critical ( this, tr ("HATA"), tr("Bu fatura yazdırılmış. Üzerinde değişiklik yapamazsınız.") );
            return false;
        }
    }

    return true;
}

/**************************************************************************************
                   FAT_FATURA_FISI::FATURA_EKRANINI_DUZENLE
***************************************************************************************/

void FAT_FATURA_FISI::FATURA_EKRANINI_DUZENLE ()
{
    m_ui->lineEdit_belge_seri->clear();

    m_ui->commaEdit_bors_alacak_durumu->setVisible( false );
    m_ui->label_cari_borc_alacak->setVisible( false );
    m_ui->lineEdit_cari_para_birimi->setVisible( false );

    if ( M_FIS->tevkifatli_fatura_mi EQ 1 ) {
        m_ui->label_tevkifatli_fatura->setHidden(false);
    }
    else {
        m_ui->label_tevkifatli_fatura->setHidden(true);
    }

    E9_KULLANICI_YETKILERI_STRUCT * KULLANICI_YETKILERI = ( E9_KULLANICI_YETKILERI_STRUCT * ) E9_GET_KULLANICI_YETKILERI();

    if ( KULLANICI_YETKILERI->fat_toplu_iskonto_yapabilir_mi EQ 0 ) {
        m_ui->label_iskonto_yuzdesi->setVisible     ( false );
        m_ui->label_iskonto->setVisible             ( false );
        m_ui->commaEdit_iskonto_tutari->setVisible  ( false );
        m_ui->commaEdit_iskonto_yuzdesi->setVisible ( false );
    }

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI                  :
        case ENUM_FAT_IRSALIYESIZ_ALIS_FATURASI                 :
        case ENUM_FAT_HAKEDIS_ALIS_FATURASI                     :
        case ENUM_FAT_VADE_FARKI_ALIS_FATURASI                  :
        case ENUM_FAT_HIZMET_ALIS_FATURASI                      :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS         :
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS          :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS :
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS                 :

            M_FIS->fatura_alis_satis_turu = ENUM_ALIS_IRSALIYESI_FATURASI;
            break;

        case ENUM_FAT_IRSALIYESIZ_SATIS_FATURASI                 :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI                  :
        case ENUM_FAT_HIZMET_SATIS_FATURASI                      :
        case ENUM_FAT_PROFORMA_SATIS_FATURASI                    :
        case ENUM_FAT_VADE_FARKI_SATIS_FATURASI                  :
        case ENUM_FAT_HAKEDIS_SATIS_FATURASI                     :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS         :
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS          :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS :
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS                 :

            M_FIS->fatura_alis_satis_turu = ENUM_SATIS_IRSALIYESI_FATURASI;
            break;

        default :
            M_FIS->fatura_alis_satis_turu = -1;
            break;
    }

    if ( M_FIS->fatura_turu EQ ENUM_FAT_VADE_FARKI_SATIS_FATURASI OR
         M_FIS->fatura_turu EQ ENUM_FAT_VADE_FARKI_ALIS_FATURASI ) {
        m_ui->button_fatura_secimi->setEnabled( false );
    }
    else {
        m_ui->button_fatura_secimi->setEnabled( true );
    }

    m_ui->label_irsaliye_belge_seri_no->setVisible( false );
    m_ui->lineEdit_irsaliye_belge_seri->setVisible( false );
    m_ui->lineEdit_irsaliye_belge_numarasi->setVisible( false );

    if ( M_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS OR M_FIS->fatura_turu EQ ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS ) {
        m_ui->label_irsaliye_belge_seri_no->setVisible( true );
        m_ui->lineEdit_irsaliye_belge_seri->setVisible( true );
        m_ui->lineEdit_irsaliye_belge_numarasi->setVisible( true );
    }

    if ( ( M_FIS->fatura_turu NE ENUM_FAT_IRSALIYELI_ALIS_FATURASI AND M_FIS->fatura_turu NE ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) OR
         ( M_FIS->fatura_turu NE ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS AND M_FIS->fatura_turu NE ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS ) ){
        m_ui->searchEdit_depo_kodu->setVisible( true );
        m_ui->lineEdit_depo_ismi->setVisible( true );
        m_ui->label_depo_kodu->setVisible( true );
        m_ui->label_depo_adi->setVisible( true );
    }

    m_ui->searchEdit_depo_kodu->SET_TEXT( "" );
    m_ui->lineEdit_depo_ismi->clear();
    m_ui->label_iptal->setVisible( false );

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS  :
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS  :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
        {

            int depo_id = SUBE_GET_UNITENIN_DEPOSU();

            QString depo_kodu, depo_adi;

            int result = SUBE_GET_DEPO_KODU_ADI( depo_id, &depo_kodu, &depo_adi );

            if ( result > 0 ) {
                m_ui->searchEdit_depo_kodu->SET_TEXT( depo_kodu );
                m_ui->lineEdit_depo_ismi->setText   ( depo_adi  );

                M_FIS->depo_id = depo_id;
            }

            if ( m_depo_islem_yetkisi NE SADECE_UNITE_DEPOSUNDA ) {
                m_ui->searchEdit_depo_kodu->setVisible(true);
                m_ui->lineEdit_depo_ismi->setVisible(true);
                m_ui->label_depo_kodu->setVisible(true);
                m_ui->label_depo_adi->setVisible(true);
            }
        }
            break;
        default :
            break;
    }

    FAT_FILL_COMBOBOX_FATURA_TURU( m_ui->comboBox_fatura_turu, M_FIS->fatura_alis_satis_turu, M_FIS->fatura_turu , m_iade_faturasi_mi );

    QString fatura_turu = m_fatura_turu_string;

    switch ( M_FIS->fatura_alis_satis_turu ) {
        case ENUM_ALIS_IRSALIYESI_FATURASI :
        default :
            fatura_turu.replace ( tr(" ( ALIŞ )"), "" );
            break;
        case ENUM_SATIS_IRSALIYESI_FATURASI :
            fatura_turu.replace ( tr(" ( SATIŞ )"), "" );
            break;
    }

    if( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
        fatura_turu.replace ( tr(" ( ALIŞ )"), "" );
        fatura_turu.replace ( tr(" ( SATIŞ )"), "" );
    }

    m_ui->comboBox_fatura_turu->setCurrentIndex ( m_ui->comboBox_fatura_turu->findText ( fatura_turu ) );

    m_ui->adakDate_fatura_tarihi->SET_DATE( QDate::currentDate() );

    m_ui->label_duzenlenme_saati->setHidden             ( true );
    m_ui->timeEdit_fatura_saati->setHidden              ( true );

    m_ui->label_iptal->setVisible                       ( false );
    m_ui->label_depo_kodu->setVisible                   ( false );
    m_ui->label_depo_adi->setVisible                    ( false );
    m_ui->searchEdit_depo_kodu->setVisible              ( false );
    m_ui->lineEdit_depo_ismi->setVisible                ( false );
    m_ui->label_depo_kodu->setVisible                   ( false );
    m_ui->groupBox_faturanin_irsaliyeleri->setVisible   ( false );
    m_ui->textEdit_irsaliye_bilgisi->setVisible         ( false );

    QString urun_hizmet_kodu_basligi;
    QString urun_hizmet_adi_basligi;

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_VADE_FARKI_ALIS_FATURASI  :
        case ENUM_FAT_VADE_FARKI_SATIS_FATURASI :
            urun_hizmet_adi_basligi  = tr("Açıklama");
            break;

        default :
            urun_hizmet_kodu_basligi = tr("Hizmet / Ürün Kodu");
            urun_hizmet_adi_basligi  = tr("Hizmet / Ürün Adı");
            break;
    }


    QString isk_oncesi_birim_fiyat_header = tr ( "Birim Fiyat" );

    if ( KULLANICI_YETKILERI->fat_satir_iskontosu_yapabilir_mi EQ 1 ) {
        isk_oncesi_birim_fiyat_header.prepend(tr ( "İsk. Öncesi \n" ) );
    }


    m_ui->tableWidget_fatura_satirlari->setHorizontalHeaderLabels(QStringList()<<"row_id"<<"fis_id"<<"order_column"
                                                            <<tr("urun_id")<<tr("stok_takip_sekli")
                                                            <<tr("seri_parti_kodu_id")<<tr("irs_satiri_id")<<tr("iade_edilne_fat_satir_id")
                                                            <<tr("fiyat_irsden_alindi") << tr("Seri No list")
                                                            <<tr( "İrs. \nStr. Seç" )<<tr ( "Fat. \nStr. Seç" )
                                                            << urun_hizmet_kodu_basligi << urun_hizmet_adi_basligi
                                                            <<tr("Seri No \n Parti Kodu")<<tr("Miktar")
                                                            <<tr("Tem.Brm.Katsayısı")<<tr("Birimi")
                                                            <<isk_oncesi_birim_fiyat_header
                                                            <<tr("İskonto %")<<tr("İsk. Tutarı")
                                                            <<tr("İsk. sonrası \n Birim fiyat")
                                                            <<tr("toplu_is_yuzdesi")
                                                            <<tr("toplu_is_sonrasi_birim_fiyat")
                                                            <<tr("toplu_is_sonrasi_satir_tutari")
                                                            <<tr("toplu_is_sonrasi_vergi_haric_tutar")
                                                            <<tr("toplu_is_sonrasi_otv_tutari")
                                                            <<tr("toplu_is_sonrasi_otv_dahil_tutar")
                                                            <<tr("toplu_is_sonrasi_kdv_tutari")
                                                            <<tr("toplu_is_sonrasi_kdv_dahil_tutar")
                                                            <<tr("KDV")<<tr("KDV %")<<tr("ÖTV %")
                                                            <<tr("Tutarı")  
                                                            <<tr("Kdv Tutarı") << tr("Kdv Dahil Tutar")
                                                            << tr(" Fatura Tutarı")
                                                            <<tr("") // fatura para birimi
                                                            <<tr("otv_tutari")<<tr("otv_dahil_tutar")
                                                            <<tr("")<<tr(""));




    // KOLON GIZLEME GOSTERME DURUMLARI AYARLANIR

    m_ui->tableWidget_fatura_satirlari->hideColumn ( TEMEL_BIRIM_KATSAYISI_COLUMN           );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( KDV_TUTARI_COLUMN                      );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( KDV_DAHIL_TUTAR_COLUMN                 );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( OTV_TUTARI_COLUMN                      );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( OTV_DAHIL_TUTAR_COLUMN                 );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( IRSALIYE_SATIRI_SECIMI_COLUMN          );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( FATURA_SATIRI_SECIMI_COLUMN            );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( IRSALIYE_SATIRI_ID_COLUMN              );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( FIYAT_IRSALIYEDEN_ALINDI_COLUMN        );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( SERI_PARTI_NO_KODU_COLUMN              );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( SERI_NO_LIST_COLUMN                    );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( ISKONTO_TUTARI_COLUMN                  );

    m_ui->tableWidget_fatura_satirlari->hideColumn ( TOPLU_ISKONTO_YUZDESI_COLUMN           );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( T_IS_SONRASI_BIRIM_FIYAT_COLUMN        );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( T_IS_SONRASI_SATIR_TUTARI_COLUMN       );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN  );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( T_IS_SONRASI_OTV_TUTARI_COLUMN         );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN    );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( T_IS_SONRASI_KDV_TUTARI_COLUMN         );
    m_ui->tableWidget_fatura_satirlari->hideColumn ( T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN    );

    m_ui->tableWidget_fatura_satirlari->hideColumn ( VERGI_HARIC_TUTAR_COLUMN    );

    m_ui->tableWidget_fatura_satirlari->showColumn ( URUN_KODU_COLUMN                       );
    m_ui->tableWidget_fatura_satirlari->showColumn ( SATIRDAKI_URUN_MIKTARI_COLUMN          );
    m_ui->tableWidget_fatura_satirlari->showColumn ( URUN_BIRIMI_COLUMN                     );
    m_ui->tableWidget_fatura_satirlari->showColumn ( ISK_ONCESI_BIRIM_FIYAT_COLUMN          );

    if ( KULLANICI_YETKILERI->fat_satir_iskontosu_yapabilir_mi EQ 0 ) {
        m_ui->tableWidget_fatura_satirlari->hideColumn ( ISKONTO_YUZDESI_COLUMN );
        m_ui->tableWidget_fatura_satirlari->hideColumn ( ISK_SONRASI_BIRIM_FIYAT_COLUMN );
    }
    else {
        m_ui->tableWidget_fatura_satirlari->showColumn ( ISK_SONRASI_BIRIM_FIYAT_COLUMN         );
        m_ui->tableWidget_fatura_satirlari->showColumn ( ISKONTO_YUZDESI_COLUMN                 );
    }

    if ( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
        m_ui->tableWidget_fatura_satirlari->showColumn ( FATURA_SATIRI_SECIMI_COLUMN );
    }

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_VADE_FARKI_ALIS_FATURASI  :
        case ENUM_FAT_VADE_FARKI_SATIS_FATURASI :
            m_ui->tableWidget_fatura_satirlari->hideColumn( FATURA_SATIRI_SECIMI_COLUMN );
            m_ui->tableWidget_fatura_satirlari->hideColumn ( URUN_KODU_COLUMN               );
            m_ui->tableWidget_fatura_satirlari->hideColumn ( SATIRDAKI_URUN_MIKTARI_COLUMN  );
            m_ui->tableWidget_fatura_satirlari->hideColumn ( URUN_BIRIMI_COLUMN             );
            m_ui->tableWidget_fatura_satirlari->hideColumn ( ISK_ONCESI_BIRIM_FIYAT_COLUMN  );
            m_ui->tableWidget_fatura_satirlari->hideColumn ( ISK_SONRASI_BIRIM_FIYAT_COLUMN );
            m_ui->tableWidget_fatura_satirlari->hideColumn ( ISKONTO_YUZDESI_COLUMN         );
            break;

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS           :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS          :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS  :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS :
            m_ui->tableWidget_fatura_satirlari->showColumn ( SERI_PARTI_NO_KODU_COLUMN );
            m_ui->tableWidget_fatura_satirlari->showColumn ( IRSALIYE_SATIRI_SECIMI_COLUMN );
            break;

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :
           m_ui->tableWidget_fatura_satirlari->showColumn ( SERI_PARTI_NO_KODU_COLUMN );

            break;
        case ENUM_FAT_HIZMET_SATIS_FATURASI:
        case ENUM_FAT_HIZMET_ALIS_FATURASI :
            m_ui->tableWidget_fatura_satirlari->hideColumn( FATURA_SATIRI_SECIMI_COLUMN );
            break;
        default :
            break;
    }

    m_ui->button_fatura_secimi->setVisible ( false  );
    m_ui->commaEdit_iade_edilecek_fis_no->setVisible( false  );
    m_ui->label_iade_edilecek_fis_no->setVisible( false  );
    m_ui->comboBox_acik_kapali_fatura->setVisible( true  );
    m_ui->searchEdit_depo_kodu->setVisible( false );
    m_ui->lineEdit_depo_ismi->setVisible( false );
    m_ui->label_depo_kodu->setVisible( false );
    m_ui->label_depo_adi->setVisible( false );

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 1 ) {
        m_ui->button_detaylar->setVisible                       ( true  );
    }

    //  FORM UZERINDEKI WIDGETLAR AYARLANIR

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS :
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS  :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI   :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI  :

            m_ui->label_duzenlenme_saati->setHidden( false );
            m_ui->timeEdit_fatura_saati->setHidden( false );
            m_ui->timeEdit_fatura_saati->setTime( QTime::currentTime() );
            m_ui->label_depo_kodu->setVisible( true );
            m_ui->label_depo_adi->setVisible( true );

            if ( m_depo_islem_yetkisi EQ SADECE_UNITE_DEPOSUNDA ) {
                m_ui->searchEdit_depo_kodu->setVisible(false);
                m_ui->lineEdit_depo_ismi->setVisible(false);
                m_ui->label_depo_kodu->setVisible(false);
                m_ui->label_depo_adi->setVisible(false);
            }
            else {
                m_ui->searchEdit_depo_kodu->setVisible(true);
                m_ui->lineEdit_depo_ismi->setVisible(true);
                m_ui->label_depo_kodu->setVisible(true);
                m_ui->label_depo_adi->setVisible(true);
            }
            break;

        case ENUM_FAT_IRSALIYESIZ_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYESIZ_SATIS_FATURASI :
        case ENUM_FAT_HAKEDIS_ALIS_FATURASI      :
        case ENUM_FAT_HAKEDIS_SATIS_FATURASI     :
        case ENUM_FAT_HIZMET_ALIS_FATURASI       :
        case ENUM_FAT_HIZMET_SATIS_FATURASI      :
        case ENUM_FAT_VADE_FARKI_ALIS_FATURASI   :
        case ENUM_FAT_VADE_FARKI_SATIS_FATURASI  :

            break;

        case ENUM_FAT_PROFORMA_SATIS_FATURASI    :

            m_ui->comboBox_acik_kapali_fatura->setVisible( false );
            m_ui->button_detaylar->setVisible( false );
            break;

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS           :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS          :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS  :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS :

            break;
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS           :
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS          :
            m_ui->groupBox_faturanin_irsaliyeleri->setVisible( true  );
            break;

        default                                  :
            break;
    }

    if ( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {

        switch( M_FIS->fatura_turu ) {
            case ENUM_FAT_VADE_FARKI_ALIS_FATURASI  :
            case ENUM_FAT_VADE_FARKI_SATIS_FATURASI :
            case ENUM_FAT_HIZMET_ALIS_FATURASI      :
            case ENUM_FAT_HIZMET_SATIS_FATURASI     :
                m_ui->button_fatura_secimi->setVisible( false  );
                m_ui->commaEdit_iade_edilecek_fis_no->setVisible( false  );
                m_ui->label_iade_edilecek_fis_no->setVisible( false  );
                break;
            default:
                m_ui->button_fatura_secimi->setVisible( true );
                m_ui->adakDate_fatura_tarihi->setVisible( true );
                m_ui->commaEdit_iade_edilecek_fis_no->setVisible( true );
                m_ui->label_iade_edilecek_fis_no->setVisible( true );
                break;
        }


    }
    else {
        M_FIS->iade_edilen_fatura_id = -1;
    }

    // KOLON GENISLIKLERI AYARLANIR

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_VADE_FARKI_ALIS_FATURASI  :
        case ENUM_FAT_VADE_FARKI_SATIS_FATURASI :

             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( IRSALIYE_SATIRI_SECIMI_COLUMN         , 50  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( FATURA_SATIRI_SECIMI_COLUMN           , 50  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( URUN_ADI_COLUMN                       , 1250);
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( SERI_PARTI_NO_KODU_COLUMN             , 75  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( KDV_DAHIL_MI_COLUMN                   , 50  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( KDV_ORANI_COLUMN                      , 50  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( OTV_ORANI_COLUMN                      , 50  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( SATIR_TUTARI_COLUMN                   , 150 );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( FATURA_PARA_BIRIMI_COLUMN             , 50  );

             SET_SETTING_NAME ( "VADE_FARKI_FATURASI" );

            break;

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS           :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS          :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS  :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS :

             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( IRSALIYE_SATIRI_SECIMI_COLUMN         , 50  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( FATURA_SATIRI_SECIMI_COLUMN           , 50  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( URUN_KODU_COLUMN                      , 200 );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( URUN_ADI_COLUMN                       , 425 );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( SERI_PARTI_NO_KODU_COLUMN             , 75  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( SATIRDAKI_URUN_MIKTARI_COLUMN         , 75  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( URUN_BIRIMI_COLUMN                    , 110 );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( ISK_ONCESI_BIRIM_FIYAT_COLUMN         , 110 );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( ISK_SONRASI_BIRIM_FIYAT_COLUMN        , 110 );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( KDV_DAHIL_MI_COLUMN                   , 50  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( KDV_ORANI_COLUMN                      , 50  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( OTV_ORANI_COLUMN                      , 50  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( SATIR_TUTARI_COLUMN                   , 150 );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( FATURA_PARA_BIRIMI_COLUMN             , 50  );

             if ( KULLANICI_YETKILERI->fat_satir_iskontosu_yapabilir_mi NE 0 ) {
                 m_ui->tableWidget_fatura_satirlari->setColumnWidth ( ISKONTO_YUZDESI_COLUMN            , 75  );
             }

             SET_SETTING_NAME                                   ( "ELLE_IRS_FATURALASTIRMA" );

            break;

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS:
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI  :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI :

             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( IRSALIYE_SATIRI_SECIMI_COLUMN         , 50  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( FATURA_SATIRI_SECIMI_COLUMN           , 50  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( URUN_KODU_COLUMN                      , 200 );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( URUN_ADI_COLUMN                       , 425 );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( SERI_PARTI_NO_KODU_COLUMN             , 75  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( SATIRDAKI_URUN_MIKTARI_COLUMN         , 75  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( URUN_BIRIMI_COLUMN                    , 110 );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( ISK_ONCESI_BIRIM_FIYAT_COLUMN         , 110 );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( ISK_SONRASI_BIRIM_FIYAT_COLUMN        , 110 );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( KDV_DAHIL_MI_COLUMN                   , 75  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( KDV_ORANI_COLUMN                      , 50  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( OTV_ORANI_COLUMN                      , 50  );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( SATIR_TUTARI_COLUMN                   , 150 );
             m_ui->tableWidget_fatura_satirlari->setColumnWidth ( FATURA_PARA_BIRIMI_COLUMN             , 50  );

             if ( KULLANICI_YETKILERI->fat_satir_iskontosu_yapabilir_mi NE 0 ) {
                 m_ui->tableWidget_fatura_satirlari->setColumnWidth ( ISKONTO_YUZDESI_COLUMN            , 75   );
             }

             SET_SETTING_NAME                                   ( "IRSALIYELI_FATURA" );

            break;

        default :

            m_ui->tableWidget_fatura_satirlari->setColumnWidth ( IRSALIYE_SATIRI_SECIMI_COLUMN          , 50  );
            m_ui->tableWidget_fatura_satirlari->setColumnWidth ( FATURA_SATIRI_SECIMI_COLUMN            , 50  );
            m_ui->tableWidget_fatura_satirlari->setColumnWidth ( URUN_KODU_COLUMN                       , 200 );
            m_ui->tableWidget_fatura_satirlari->setColumnWidth ( URUN_ADI_COLUMN                        , 450 );
            m_ui->tableWidget_fatura_satirlari->setColumnWidth ( SERI_PARTI_NO_KODU_COLUMN              , 75  );
            m_ui->tableWidget_fatura_satirlari->setColumnWidth ( SATIRDAKI_URUN_MIKTARI_COLUMN          , 75  );
            m_ui->tableWidget_fatura_satirlari->setColumnWidth ( URUN_BIRIMI_COLUMN                     , 110 );
            m_ui->tableWidget_fatura_satirlari->setColumnWidth ( ISK_ONCESI_BIRIM_FIYAT_COLUMN          , 110 );
            m_ui->tableWidget_fatura_satirlari->setColumnWidth ( ISK_SONRASI_BIRIM_FIYAT_COLUMN         , 110 );
            m_ui->tableWidget_fatura_satirlari->setColumnWidth ( KDV_DAHIL_MI_COLUMN                    , 50  );
            m_ui->tableWidget_fatura_satirlari->setColumnWidth ( KDV_ORANI_COLUMN                       , 50  );
            m_ui->tableWidget_fatura_satirlari->setColumnWidth ( OTV_ORANI_COLUMN                       , 50  );
            m_ui->tableWidget_fatura_satirlari->setColumnWidth ( SATIR_TUTARI_COLUMN                    , 150 );
            m_ui->tableWidget_fatura_satirlari->setColumnWidth ( FATURA_PARA_BIRIMI_COLUMN              , 50  );

            if ( KULLANICI_YETKILERI->fat_satir_iskontosu_yapabilir_mi NE 0 ) {
                m_ui->tableWidget_fatura_satirlari->setColumnWidth ( ISKONTO_YUZDESI_COLUMN             , 75  );
            }

            SET_SETTING_NAME ( "GENEL_FATURA_FISI" );

            break;
    }

    switch ( M_FIS->fatura_alis_satis_turu ) {
        case ENUM_ALIS_IRSALIYESI_FATURASI :
        default :
            fatura_turu.replace ( tr(" ( ALIŞ )"), "" );
            break;
        case ENUM_SATIS_IRSALIYESI_FATURASI :
            fatura_turu.replace ( tr(" ( SATIŞ )"), "" );
            break;
    }

    switch ( M_FIS->fatura_turu ) {

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS :
            SET_PAGE_TITLE( tr ( "FAT - İRSALİYE KISMEN FATURALAŞTIRMA ( ALIŞ )") );
            SET_HELP_PAGE("fatura-islemleri_irsaliyeyi-kismen-faturalastirma");
            break;
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS:
            SET_PAGE_TITLE( tr ( "FAT - İRSALİYE KISMEN FATURALAŞTIRMA ( SATIŞ )" ) );
            SET_HELP_PAGE("fatura-islemleri_satis-irsaliyeyi-kısmen-faturalastirma");
            break;
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS:
            SET_PAGE_TITLE( tr ( "FAT - İRSALİYE FATURALAŞTIRMA ( ALIŞ )" ) );
            SET_HELP_PAGE("fatura-islemleri_alis-irsaliye-faturalastirma");
            break;
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS:
            SET_PAGE_TITLE( tr ( "FAT - İRSALİYE FATURALAŞTIRMA ( SATIŞ )" ) );
            SET_HELP_PAGE("fatura-islemleri_satis-irsaliye-faturalastirma");
            break;
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS:
            SET_PAGE_TITLE( tr ( "FAT - KONSİNYE İRSALİYESİNİ FATURALAŞTIRMA ( ALIŞ )" ) );
            SET_HELP_PAGE("fatura-islemleri_alis-konsinye-irsaliyesini-faturalastirma");
            break;
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS:
            SET_PAGE_TITLE( tr ( "FAT - KONSİNYE İRSALİYESİNİ FATURALAŞTIRMA ( SATIŞ )" ) );
            SET_HELP_PAGE("fatura-islemleri_satis-konsinye-irsaliyesini-faturalastirma");
            break;

        default : {

            if ( M_FIS->fatura_alis_satis_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI ) {
                if ( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
                    SET_PAGE_TITLE( tr ( "FAT - ALIŞTAN İADE FATURALARI" ) );
                    SET_HELP_PAGE("fatura-islemleri_alistan-iade-faturalari");
                }
                else {
                    SET_PAGE_TITLE( tr ( "FAT - ALIŞ FATURALARI" ) );
                    SET_HELP_PAGE("fatura-islemleri_alis-faturalari");
                }
            }
            else {
                if ( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
                    SET_PAGE_TITLE( tr ( "FAT - SATIŞTAN İADE FATURALARI" ) );
                    SET_HELP_PAGE("fatura-islemleri_satistan-iade-faturalari");
                }
                else {
                    SET_PAGE_TITLE( tr ( "FAT - SATIŞ FATURALARI" ) );
                    SET_HELP_PAGE("fatura-islemleri_satis-faturalari");
                }
            }
            break;
        }
    }

    bool sube_is_visible = false;
    if( SUBE_GET_DEPO_COUNT( SUBE_GET_SUBE_ID() ) > 1 ) {
        sube_is_visible = true;
    }

        m_ui->searchEdit_depo_kodu->setVisible( sube_is_visible );
        m_ui->label_depo_kodu->setVisible( sube_is_visible );
        m_ui->label_depo_adi->setVisible( sube_is_visible );
        m_ui->lineEdit_depo_ismi->setVisible( sube_is_visible );
}

/**************************************************************************************
                   FAT_FATURA_FISI::SAVER_BUTTON_CLICKED
***************************************************************************************/

void FAT_FATURA_FISI::SAVER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_fis_id )
{
    SQL_QUERY query    ( DB );

    if ( p_button EQ m_ui->button_irsaliye_secimi ) {
        QList< int > irsaliye_id_list;
        //! IRSALIYE MULTI SECIM OLDU
        OPEN_FAT_IRSALIYE_ARAMA ( m_ui->adakDate_fatura_tarihi->DATE(), M_FIS->cari_hesap_id, M_FIS->fatura_alis_satis_turu, &irsaliye_id_list, this );

        DB->START_TRANSACTION();
        //! For begin
        for (int i = 0; i < irsaliye_id_list.size(); ++i) {

            int order_number = 0;

            query.PREPARE_SELECT("fat_fatura_satirlari" , "order_number" , "fis_id = :fis_id");

            query.SET_VALUE(":fis_id", p_fis_id );

            if ( query.SELECT("order_number DESC" , 0,1) > 0 ) {
                query.NEXT();
                order_number = query.VALUE(0).toInt();
            }

            order_number += 1024;

            STK_IRS_FIS_SATIR_STRUCT * IRS_FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

            STK_IRS_CLEAR_SATIR_STRUCT ( IRS_FIS_SATIRI );

            QList <int> IRS_SATIRI_ID_LIST = STK_GET_FIS_SATIRI_ID_LIST( irsaliye_id_list.at( i ) );

            if ( IRS_SATIRI_ID_LIST.isEmpty() EQ true ) {
                DB->CANCEL_TRANSACTION();
                return;
            }

            for ( int j = 0 ; j < IRS_SATIRI_ID_LIST.size() ; j++ ) {

                int irs_satiri_id = IRS_SATIRI_ID_LIST.at(j);

                STK_IRS_SATIR_BILGILERINI_OKU( irs_satiri_id, IRS_FIS_SATIRI);

                double urun_miktari       = IRS_FIS_SATIRI->satirdaki_urun_miktari;
                int    urun_id            = IRS_FIS_SATIRI->urun_id;

                if ( urun_id < 1 AND urun_miktari EQ 0 ) {
                    continue;
                }

                FATURA_CLEAR_FIS_SATIRI_STRUCT ( M_FIS_SATIRI );

                M_FIS_SATIRI->fis_id                                  = p_fis_id;
                M_FIS_SATIRI->urun_id                                 = urun_id;
                M_FIS_SATIRI->urun_adi                                = IRS_FIS_SATIRI->urun_adi;
                M_FIS_SATIRI->satirdaki_urun_miktari                  = urun_miktari;
                M_FIS_SATIRI->urun_birimi                             = IRS_FIS_SATIRI->urun_birimi;
                M_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati           = IRS_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati;
                M_FIS_SATIRI->isk_oncesi_urun_birim_fiyati            = IRS_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati;
                M_FIS_SATIRI->satir_tutari                            = IRS_FIS_SATIRI->satir_tutari;
                M_FIS_SATIRI->doviz_id                                = IRS_FIS_SATIRI->doviz_id;
                M_FIS_SATIRI->doviz_kodu                              = DVZ_GET_DOVIZ_KODU( IRS_FIS_SATIRI->doviz_id);
                M_FIS_SATIRI->satir_vergi_haric_tutar                 = IRS_FIS_SATIRI->satir_vergi_haric_tutar;
                M_FIS_SATIRI->kdv_orani                               = IRS_FIS_SATIRI->kdv_orani;
                M_FIS_SATIRI->satir_kdv_tutari                        = IRS_FIS_SATIRI->satir_kdv_tutari;
                M_FIS_SATIRI->satir_kdv_dahil_tutar                   = IRS_FIS_SATIRI->satir_kdv_dahil_tutar;
                M_FIS_SATIRI->kdv_dahil_mi                            = IRS_FIS_SATIRI->kdv_dahil_mi;
                M_FIS_SATIRI->order_number                            = order_number;
                M_FIS_SATIRI->temel_birim_katsayisi                   = IRS_FIS_SATIRI->temel_birim_katsayisi;
                M_FIS_SATIRI->otv_orani                               = IRS_FIS_SATIRI->otv_orani;
                M_FIS_SATIRI->satir_otv_tutari                        = IRS_FIS_SATIRI->satir_otv_tutari;
                M_FIS_SATIRI->satir_otv_dahil_tutar                   = IRS_FIS_SATIRI->satir_otv_dahil_tutar;
                M_FIS_SATIRI->irsaliye_satiri_id                      = irs_satiri_id;
                M_FIS_SATIRI->toplu_iskonto_yuzdesi                   = IRS_FIS_SATIRI->toplu_iskonto_yuzdesi;
                M_FIS_SATIRI->t_is_son_birim_fiyati                   = IRS_FIS_SATIRI->t_is_sonrasi_birim_fiyat;
                M_FIS_SATIRI->t_is_son_satir_tutari                   = IRS_FIS_SATIRI->t_is_sonrasi_satir_tutari;
                M_FIS_SATIRI->t_is_son_str_vergi_haric_tutar          = IRS_FIS_SATIRI->t_is_sonrasi_vergi_haric_tutar;
                M_FIS_SATIRI->t_is_son_satir_otv_tutari               = IRS_FIS_SATIRI->t_is_sonrasi_otv_tutari;
                M_FIS_SATIRI->t_is_son_satir_otv_dahil_tutar          = IRS_FIS_SATIRI->t_is_sonrasi_otv_dahil_tutar;
                M_FIS_SATIRI->t_is_son_satir_kdv_tutari               = IRS_FIS_SATIRI->t_is_sonrasi_kdv_tutari;
                M_FIS_SATIRI->t_is_son_satir_kdv_dahil_tutar          = IRS_FIS_SATIRI->t_is_sonrasi_kdv_dahil_tutar;
                M_FIS_SATIRI->irsaliyelenen_miktar                    = IRS_FIS_SATIRI->satirdaki_urun_miktari;

                FATURA_FIS_SATIRI_EKLE ( M_FIS, M_FIS_SATIRI );
            }

            STK_UPDATE_IRSALIYE_FATURALASTIRILDI_MI( irsaliye_id_list.at( i ), 1 );

            query.PREPARE_UPDATE ( "fat_faturalar","fis_id", "fatura_irsaliyelestirildi_mi ",
                                   "fis_id = :fis_id");
            query.SET_VALUE      ( ":fatura_irsaliyelestirildi_mi" , 1);
            query.SET_VALUE      ( ":fis_id", p_fis_id );
            query.UPDATE         ();

            query.PREPARE_INSERT("fat_irs_baglantisi" , "kayit_id" , "fatura_id , irsaliye_id");

            query.SET_VALUE      ( ":fatura_id"   , p_fis_id      );
            query.SET_VALUE      ( ":irsaliye_id" , irsaliye_id_list.at( i ) );

            query.INSERT();
        }
        //! enf of For
        DB->COMMIT_TRANSACTION();

        REFRESH_FIS_RECORD ();

    }
    else if ( p_button EQ m_ui->button_yazdir ) {

        QString fat_belge_seri, fat_belge_numarasi;

        if ( M_FIS->yazdirildi_mi NE 1 ) {

            SUBE_UNITE_BILGILERI_STRUCT UNITE_BILG;
            SUBE_UNITE_BILGILERI_OKU( &UNITE_BILG, SUBE_GET_UNITE_ID() );

            fat_belge_seri     = m_ui->lineEdit_belge_seri->text();
            fat_belge_numarasi = m_ui->lineEdit_belge_numarasi->text();

            if ( m_ui->lineEdit_belge_seri->text().isEmpty() EQ true OR m_ui->lineEdit_belge_numarasi->text().isEmpty() EQ true ) {
                if ( m_fatura_turu EQ ENUM_FAT_IRSALIYELI_ALIS_FATURASI OR m_fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) {
                    fat_belge_seri      = UNITE_BILG.irsaliyeli_fat_belge_seri;
                    fat_belge_numarasi  = UNITE_BILG.irsaliyeli_fat_belge_numarasi;
                }
                else {
                    fat_belge_seri      = UNITE_BILG.fatura_belge_seri;
                    fat_belge_numarasi  = UNITE_BILG.fatura_belge_numarasi;
                }
            }

            int secim = MSG_YES_NO( tr( "Fatura; \n"
                                        "Belge Seri          : %1 \n"
                                        "Belge Numarası : %2 \n"
                                        "ile yazdırılacaktır. Onaylıyormusunuz ?").arg( fat_belge_seri ).arg( fat_belge_numarasi ), m_ui->lineEdit_belge_seri );

            if ( secim EQ ADAK_NO ) {
                return;
            }
        }
        else {
            int secim = MSG_YES_NO( tr( "Bu fatura daha önce yazdırıldı. Tekrar yazdırmak istiyormusunuz ?" ), NULL );

            if ( secim EQ ADAK_NO ) {
                return;
            }
        }
        // YAZDIRMA ISLEMI
        if ( M_FIS->yazdirildi_mi NE 1 ) {
            m_ui->lineEdit_belge_seri->setText( fat_belge_seri );
            m_ui->lineEdit_belge_numarasi->setText( fat_belge_numarasi );
        }

        PRINT_FAT_FATURA_BELGESI ( p_fis_id ,this);

        DB->START_TRANSACTION();

        query.PREPARE_UPDATE        ( "fat_faturalar","fis_id", "yazdirildi_mi" ,"fis_id = :fis_id");
        query.SET_VALUE             ( ":yazdirildi_mi"  , 1);
        query.SET_VALUE             ( ":fis_id"         , p_fis_id );
        query.UPDATE                ();

        M_FIS->yazdirildi_mi = 1;

        DB->COMMIT_TRANSACTION();

    }
    else if ( p_button EQ m_ui->button_detaylar ) {

        if ( OPEN_FAT_FATURA_DETAYLARI_BATCH ( M_FATURA_DETAYLARI, p_fis_id, M_FIS->fatura_alis_satis_turu, this, m_iade_faturasi_mi ) EQ true ) {

            m_ui->textEdit_kdv_tutarlari->clear();
            m_ui->textEdit_otv_tutarlari->clear();

            REFRESH_FIS_RECORD ();

            CARI_FIS_STRUCT         * CARI_FIS          = new CARI_FIS_STRUCT;
            CARI_FIS_SATIRI_STRUCT  * CARI_FIS_SATIRI   = new CARI_FIS_SATIRI_STRUCT;
            MUH_FIS_STRUCT          * MUH_FIS           = new MUH_FIS_STRUCT;
            MUH_FIS_SATIRI_STRUCT   * MUH_FIS_SATIRI    = new MUH_FIS_SATIRI_STRUCT;


            switch ( M_FIS->fatura_turu ) {

                case ENUM_FAT_PROFORMA_SATIS_FATURASI :
                    break;

                default  :
                    DB->START_TRANSACTION();
                    FAT_FATURA_ENT_FISLERINI_OLUSTUR( M_FIS, p_fis_id );
                    DB->COMMIT_TRANSACTION();
                    M_FATURA_DETAYLARI->muh_fis_no  = MUH_FIS->fis_no;
                    M_FATURA_DETAYLARI->cari_fis_no = CARI_FIS->fis_no;

                    break;
            }

            delete CARI_FIS;
            delete CARI_FIS_SATIRI;
            delete MUH_FIS;
            delete MUH_FIS_SATIRI;
        }
    }
    else if ( p_button EQ m_ui->pushButton_urettigi_fisler ) {
        E9_URETILEN_FIS_SEC( M_FIS->program_id, M_FIS->modul_id, p_fis_id, this );
    }
}

/**************************************************************************************
                   FAT_FATURA_FISI::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int FAT_FATURA_FISI::CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button )
{

    if ( p_button EQ m_ui->button_fatura_secimi ) {

        REFRESH_FIS_RECORD ();

        SQL_QUERY query ( DB );

        if ( M_FIS->cari_hesap_id < 1 ) {
            MSG_WARNING( tr("Önce cari hesabı seçmelisiniz"), m_ui->searchEdit_cari_hesap_kodu );
            return ADAK_RECORD_UNCHANGED;
        }

        int iade_edilen_fatura_id = OPEN_IADE_FATURA_SECIMI ( M_FIS->cari_hesap_id, M_FIS->fatura_alis_satis_turu, M_FIS->fatura_turu, this );

        if ( iade_edilen_fatura_id > 0 ) {

            query.PREPARE_SELECT ( "fat_faturalar","fis_no" ,"fis_id = :fis_id" );
            query.SET_VALUE      ( ":fis_id", iade_edilen_fatura_id );

            if ( query.SELECT() EQ 0 ) {
                MSG_WARNING( tr ( "Fatura bilgileri okunamadı." ), NULL );
                return ADAK_RECORD_UNCHANGED;
            }

            query.NEXT();

            M_FIS->iade_edilen_fatura_id = iade_edilen_fatura_id;
            m_ui->commaEdit_iade_edilecek_fis_no->SET_INTEGER ( query.VALUE(0).toInt() );
            return ADAK_RECORD_CHANGED;
        }
    }


    return ADAK_RECORD_UNCHANGED;
}


/**************************************************************************************
                   FAT_FATURA_FISI::LINE_CHANGER_BUTTON_CLICKED
***************************************************************************************/

int FAT_FATURA_FISI::LINE_CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_row_number )
{
    if ( M_FIS->cari_hesap_id < 1 ) {
        MSG_WARNING( tr("Önce cari hesabı seçmelisiniz"), NULL );
        return ADAK_RECORD_UNCHANGED;
    }

    SQL_QUERY query ( DB );

    QSearchEdit * searchEdit_urun_kodu                      = ( QSearchEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_KODU_COLUMN                      );
    QLineEdit   * lineEdit_urun_id                          = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN                        );
    QLineEdit   * lineEdit_urun_adi                         = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_ADI_COLUMN                       );
    QCommaEdit  * commaEdit_isk_oncesi_birim_fiyat          = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN         );
    QCommaEdit  * commaEdit_isk_sonrasi_birim_fiyat         = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN        );
    QCommaEdit  * commaEdit_iskonto_yuzdesi                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_YUZDESI_COLUMN                );
    QCommaEdit  * commaEdit_iskonto_tutari                  = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_TUTARI_COLUMN                 );
    QLineEdit   * lineEdit_stok_takip_sekli                 = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, STOK_TAKIP_SEKLI_COLUMN               );
    QComboBox   * comboBox_urun_birimleri                   = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN                    );
    QPushButton * pushButton_seri_parti_kodu                = ( QPushButton * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SERI_PARTI_NO_KODU_COLUMN             );
    QCommaEdit  * commaEdit_satirdaki_urun_miktari          = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIRDAKI_URUN_MIKTARI_COLUMN         );
    QCommaEdit  * commaEdit_satir_tutari                    = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN                   );
    QLineEdit   * lineEdit_parti_kodu_id                    = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, PARTI_ID_COLUMN                       );
    QLineEdit   * lineedit_para_birimi                      = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, FATURA_PARA_BIRIMI_COLUMN             );
    QCommaEdit  * commaEdit_vergi_haric_tutar               = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTAR_COLUMN              );
    QComboBox   * comboBox_kdv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_ORANI_COLUMN                      );
    QComboBox   * comboBox_kdv_dahil_mi                     = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN                   );
    QCommaEdit  * commaEdit_kdv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_TUTAR_COLUMN                );
    QCommaEdit  * commaEdit_kdv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_TUTARI_COLUMN                     );
    QComboBox   * comboBox_otv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_ORANI_COLUMN                      );
    QCommaEdit  * commaEdit_otv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_DAHIL_TUTAR_COLUMN                );
    QCommaEdit  * commaEdit_otv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_TUTARI_COLUMN                     );
    QLineEdit   * lineEdit_irsaliye_satiri_id               = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, IRSALIYE_SATIRI_ID_COLUMN             );
    QCommaEdit  * commaEdit_temel_birim_katsayisi           = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, TEMEL_BIRIM_KATSAYISI_COLUMN          );
    QLineEdit   * lineEdit_fiyat_irsaliyeden_alindi         = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, FIYAT_IRSALIYEDEN_ALINDI_COLUMN       );
    QLineEdit   * lineEdit_iade_edilen_fat_satiri_id        = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, IADE_EDILEN_FAT_SATIR_ID_COLUMN       );
    QCommaEdit  * commaEdit_t_is_sonrasi_birim_fiyat        = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget  ( p_row_number, T_IS_SONRASI_BIRIM_FIYAT_COLUMN       );
    QCommaEdit  * commaEdit_t_is_sonrasi_satir_tutari       = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget  ( p_row_number, T_IS_SONRASI_SATIR_TUTARI_COLUMN      );
    QCommaEdit  * commaEdit_t_is_sonrasi_vergi_haric_tutar  = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget  ( p_row_number, T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN );
    QCommaEdit  * commaEdit_t_is_sonrasi_otv_tutari         = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget  ( p_row_number, T_IS_SONRASI_OTV_TUTARI_COLUMN        );
    QCommaEdit  * commaEdit_t_is_sonrasi_otv_dahil_tutar    = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget  ( p_row_number, T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN   );
    QCommaEdit  * commaEdit_t_is_sonrasi_kdv_tutari         = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget  ( p_row_number, T_IS_SONRASI_KDV_TUTARI_COLUMN        );
    QCommaEdit  * commaEdit_t_is_sonrasi_kdv_dahil_tutar    = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget  ( p_row_number, T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN   );
    QPushButton * button_irsaliye_satiri_secimi             = ( QPushButton * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, IRSALIYE_SATIRI_SECIMI_COLUMN        );
    QPushButton * button_fatura_satiri_secimi               = ( QPushButton * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, FATURA_SATIRI_SECIMI_COLUMN          );
    QLineEdit   * lineEdit_seri_no_list                     = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SERI_NO_LIST_COLUMN                   );
   
    int irsaliye_satiri_id                          = 0;
    int fatura_satiri_id                            = 0;
    int         urun_id                             = 0;
    QString     urun_adi                            = "";
    double      satirdaki_urun_miktari              = 0;
    QString     urun_birimi                         = "";
    double      temel_birim_katsayisi               = 0;
    double      isk_oncesi_urun_birim_fiyati        = 0;
    double      isk_sonrasi_urun_birim_fiyati       = 0;
    double      iskonto_yuzdesi                     = 0;
    double      iskonto_tutari                      = 0;
    double      satir_tutari                        = 0;
    int         doviz_id                            = 0;
    double      vergi_haric_tutar                   = 0;
    QString     kdv_orani                           = "";
    double      kdv_tutari                          = 0;
    double      kdv_dahil_tutar                     = 0;
    QString     otv_orani                           = 0;
    double      otv_tutari                          = 0;
    double      otv_dahil_tutar                     = 0;
    int         kdv_dahil_mi                        = 0;
    double      t_is_son_satir_kdv_tutari           = 0;
    double      t_is_son_satir_kdv_dahil_tutar      = 0;
    double      t_is_son_str_vergi_haric_tutar      = 0;
    double      t_is_son_satir_otv_tutari           = 0;
    double      t_is_son_satir_otv_dahil_tutar      = 0;
    double      t_is_son_birim_fiyati               = 0;
    double      t_is_son_satir_tutari               = 0;
    int         parti_id                            = 0;
    double      iade_edilen_miktar                  = 0.0;
    double      faturalanan_miktar                  = 0.0;

    if ( p_button EQ button_irsaliye_satiri_secimi ) {

        irsaliye_satiri_id = OPEN_FAT_IRSALIYE_SATIR_SECIMI ( m_ui->adakDate_fatura_tarihi->DATE(), M_FIS->cari_hesap_id, M_FIS->fatura_turu, this );
        if ( irsaliye_satiri_id < 1 ) {
            return ADAK_RECORD_UNCHANGED;
        }

        STK_IRS_FIS_SATIR_STRUCT * IRS_FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

        STK_IRS_CLEAR_SATIR_STRUCT(IRS_FIS_SATIRI);

        int result = STK_IRS_SATIR_BILGILERINI_OKU(irsaliye_satiri_id , IRS_FIS_SATIRI);


        if ( result EQ 0 ) {
            MSG_WARNING(  tr("İrsaliye satır bilgileri okunanamadı."), NULL );
            return ADAK_RECORD_UNCHANGED;
        }

        lineEdit_irsaliye_satiri_id->setText ( QVariant ( irsaliye_satiri_id ).toString() );

        urun_id                         = IRS_FIS_SATIRI->urun_id;
        urun_adi                        = IRS_FIS_SATIRI->urun_adi;
        satirdaki_urun_miktari          = IRS_FIS_SATIRI->satirdaki_urun_miktari;
        urun_birimi                     = IRS_FIS_SATIRI->urun_birimi;
        temel_birim_katsayisi           = IRS_FIS_SATIRI->temel_birim_katsayisi;
        isk_oncesi_urun_birim_fiyati    = IRS_FIS_SATIRI->isk_oncesi_urun_birim_fiyati;
        isk_sonrasi_urun_birim_fiyati   = IRS_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati;
        iskonto_tutari                  = IRS_FIS_SATIRI->iskonto_tutari;
        iskonto_yuzdesi                 = IRS_FIS_SATIRI->iskonto_yuzdesi;
        satir_tutari                    = IRS_FIS_SATIRI->satir_tutari;
        doviz_id                        = IRS_FIS_SATIRI->doviz_id;
        vergi_haric_tutar               = IRS_FIS_SATIRI->satir_vergi_haric_tutar;
        kdv_orani                       = QVariant(  IRS_FIS_SATIRI->kdv_orani ).toString();
        kdv_tutari                      = IRS_FIS_SATIRI->satir_kdv_tutari;
        kdv_dahil_tutar                 = IRS_FIS_SATIRI->satir_kdv_dahil_tutar;
        otv_orani                       = QVariant( IRS_FIS_SATIRI->otv_orani ).toString();
        otv_tutari                      = IRS_FIS_SATIRI->satir_otv_tutari;
        otv_dahil_tutar                 = IRS_FIS_SATIRI->satir_otv_dahil_tutar;
        kdv_dahil_mi                    = IRS_FIS_SATIRI->kdv_dahil_mi;
        t_is_son_satir_kdv_tutari       = IRS_FIS_SATIRI->t_is_sonrasi_kdv_tutari;
        t_is_son_satir_kdv_dahil_tutar  = IRS_FIS_SATIRI->t_is_sonrasi_kdv_dahil_tutar;
        t_is_son_str_vergi_haric_tutar  = IRS_FIS_SATIRI->t_is_sonrasi_vergi_haric_tutar;
        t_is_son_satir_otv_tutari       = IRS_FIS_SATIRI->t_is_sonrasi_otv_tutari;
        t_is_son_satir_otv_dahil_tutar  = IRS_FIS_SATIRI->t_is_sonrasi_otv_dahil_tutar;
        t_is_son_birim_fiyati           = IRS_FIS_SATIRI->t_is_sonrasi_birim_fiyat;
        t_is_son_satir_tutari           = IRS_FIS_SATIRI->t_is_sonrasi_satir_tutari;
        parti_id                        = IRS_FIS_SATIRI->parti_id;
        iade_edilen_miktar              = IRS_FIS_SATIRI->iade_edilen_miktar;
        faturalanan_miktar              = IRS_FIS_SATIRI->faturalanan_miktar;

        //! LINE_CHECK_VAR 'DA KONTROL ICIN LAZIM
        m_irs_faturalastirma_str_max_urun_sayi = satirdaki_urun_miktari;

        if( STK_GET_URUN_TAKIP_SEKLI( urun_id ) EQ ENUM_BASIT_TAKIP ) {
            SET_FOCUS( commaEdit_satirdaki_urun_miktari );
        }
        else {
            SET_FOCUS( pushButton_seri_parti_kodu );
        }

    }
    else if ( p_button EQ button_fatura_satiri_secimi )  {

        if ( M_FIS->iade_edilen_fatura_id < 1 ) {
            MSG_WARNING(  tr ( "İade edilecek fatura seçimi yapılmamış" ), NULL );
            return ADAK_RECORD_UNCHANGED;
        }

        fatura_satiri_id = OPEN_FAT_IADE_SATIR_SECIMI ( M_FIS->iade_edilen_fatura_id, this );

        irsaliye_satiri_id = STK_GET_FAT_IRS_STR_BAGLANTISI_IRSALIYE_SATIRI_ID( fatura_satiri_id );

        if ( fatura_satiri_id < 1 ) {
            return ADAK_RECORD_UNCHANGED;
        }



        query.PREPARE_SELECT("fat_fatura_satirlari" , "urun_id, urun_adi, satirdaki_urun_miktari, urun_birimi, "
                             "temel_birim_katsayisi, isk_oncesi_urun_birim_fiyati, "
                             "isk_sonrasi_urun_birim_fiyati, satir_iskonto_yuzdesi, satir_iskonto_tutari, "
                             "satir_tutari, doviz_id, satir_vergi_haric_tutar, kdv_orani, "
                             "satir_kdv_tutari, satir_kdv_dahil_tutar, otv_orani, satir_otv_tutari, "
                             "satir_otv_dahil_tutar, kdv_dahil_mi, iade_edilen_miktar, "
                             "t_is_son_satir_kdv_tutari, t_is_son_satir_kdv_dahil_tutar, "
                             "t_is_son_str_vergi_haric_tutar, t_is_son_satir_otv_tutari, "
                             "t_is_son_satir_otv_dahil_tutar, t_is_son_birim_fiyati, t_is_son_satir_tutari ",
                             "fis_satiri_id = :fis_satiri_id");

        query.SET_VALUE ( ":fis_satiri_id", fatura_satiri_id );
        if ( query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr("Fatura satır bilgileri okunanamadı."), NULL );
            return ADAK_RECORD_UNCHANGED;
        }
        lineEdit_iade_edilen_fat_satiri_id->setText ( QVariant ( fatura_satiri_id ).toString() );

        query.NEXT();

        urun_id                             = query.VALUE ( "urun_id"                           ).toInt();
        urun_adi                            = query.VALUE ( "urun_adi"                          ).toString();
        satirdaki_urun_miktari              = query.VALUE ( "satirdaki_urun_miktari"            ).toDouble();
        urun_birimi                         = query.VALUE ( "urun_birimi"                       ).toString();
        temel_birim_katsayisi               = query.VALUE ( "temel_birim_katsayisi"             ).toDouble();
        isk_oncesi_urun_birim_fiyati        = query.VALUE ( "isk_oncesi_urun_birim_fiyati"      ).toDouble();
        isk_sonrasi_urun_birim_fiyati       = query.VALUE ( "isk_sonrasi_urun_birim_fiyati"     ).toDouble();
        iskonto_yuzdesi                     = query.VALUE ( "satir_iskonto_yuzdesi"             ).toDouble();
        iskonto_tutari                      = query.VALUE ( "satir_iskonto_tutari"              ).toDouble();
        satir_tutari                        = query.VALUE ( "satir_tutari"                      ).toDouble();
        doviz_id                            = query.VALUE ( "doviz_id"                          ).toInt();
        vergi_haric_tutar                   = query.VALUE ( "satir_vergi_haric_tutar"           ).toDouble();
        kdv_orani                           = query.VALUE ( "kdv_orani"                         ).toString();
        kdv_tutari                          = query.VALUE ( "satir_kdv_tutari"                  ).toDouble();
        kdv_dahil_tutar                     = query.VALUE ( "satir_kdv_dahil_tutar"             ).toDouble();
        otv_orani                           = query.VALUE ( "otv_orani"                         ).toString();
        otv_tutari                          = query.VALUE ( "satir_otv_tutari"                  ).toDouble();
        otv_dahil_tutar                     = query.VALUE ( "satir_otv_dahil_tutar"             ).toDouble();
        kdv_dahil_mi                        = query.VALUE ( "kdv_dahil_mi"                      ).toInt();
        t_is_son_satir_kdv_tutari           = query.VALUE ( "t_is_son_satir_kdv_tutari"         ).toDouble();
        t_is_son_satir_kdv_dahil_tutar      = query.VALUE ( "t_is_son_satir_kdv_dahil_tutar"    ).toDouble();
        t_is_son_str_vergi_haric_tutar      = query.VALUE ( "t_is_son_str_vergi_haric_tutar"    ).toDouble();
        t_is_son_satir_otv_tutari           = query.VALUE ( "t_is_son_satir_otv_tutari"         ).toDouble();
        t_is_son_satir_otv_dahil_tutar      = query.VALUE ( "t_is_son_satir_otv_dahil_tutar"    ).toDouble();
        t_is_son_birim_fiyati               = query.VALUE ( "t_is_son_birim_fiyati"             ).toDouble();
        t_is_son_satir_tutari               = query.VALUE ( "t_is_son_satir_tutari"             ).toDouble();
        iade_edilen_miktar                  = query.VALUE ( "iade_edilen_miktar"                ).toDouble();
        //faturalanan_miktar                  = query.VALUE ( "faturalanan_miktar"                ).toDouble();
        //parti_id                            = query.VALUE("parti_id").toInt();

        if( STK_GET_URUN_TAKIP_SEKLI( urun_id ) EQ ENUM_BASIT_TAKIP ) {
            SET_FOCUS( commaEdit_satirdaki_urun_miktari );
        }
        else {
            SET_FOCUS( pushButton_seri_parti_kodu );
        }

        m_ui->button_fatura_secimi->setEnabled( false );

    }
    else if( p_button EQ pushButton_seri_parti_kodu ) {

        int fatura_satiri_id = -1;
        QLineEdit * line_edit_row_id = (QLineEdit *) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ROW_ID_COLUMN );

        if( line_edit_row_id NE NULL ) {
            // satir kaydedilmis update ediliyor demek
            fatura_satiri_id = line_edit_row_id->text().toInt();
        }

        int stok_takip_sekli = lineEdit_stok_takip_sekli->text().toInt();

        int fatura_alis_satis_turu = M_FIS->fatura_alis_satis_turu;

        M_FIS_SATIRI->urun_id = lineEdit_urun_id->text().toInt();

        // IADE DE SECIM EKRANI GELMESI ICIN
        if ( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
            fatura_alis_satis_turu = ENUM_SATIS_IRSALIYESI_FATURASI;
        }

        int seri_no_fatura_durumu = -1;
        switch( M_FIS->fatura_turu ) {

            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS            :
            case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS             :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS   :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS             :
            case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS              :
            case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS    :
                fatura_alis_satis_turu = ENUM_SATIS_IRSALIYESI_FATURASI;
                seri_no_fatura_durumu  = fatura_satiri_id;
                break;
            default:
                break;
        }

        switch ( stok_takip_sekli ) {
            case ENUM_BASIT_TAKIP       :
            default                     :
                return ADAK_RECORD_UNCHANGED;
                break;
            case ENUM_SERI_NO_ILE_TAKIP : {
                QStringList seri_no_list;
                if( lineEdit_seri_no_list->text().isEmpty() EQ false ) {
                    seri_no_list = lineEdit_seri_no_list->text().split(",");
                }
                switch ( fatura_alis_satis_turu ) {
                    case ENUM_ALIS_IRSALIYESI_FATURASI : {
                        OPEN_STOK_GELEN_SERI_NO_BATCH( M_FIS->depo_id, M_FIS_SATIRI->urun_id, &seri_no_list, this );
                        }
                        break;

                    case ENUM_SATIS_IRSALIYESI_FATURASI :
                    default                                     : {
                        if( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
                            if( M_FIS->fatura_alis_satis_turu EQ ENUM_SATIS_IRSALIYESI_FATURASI ) {
                                STOK_GIDEN_SERI_NO_SEC( M_FIS->depo_id, M_FIS_SATIRI->urun_id, &seri_no_list, ENUM_URUN_DEPODA_DEGIL, true, seri_no_fatura_durumu, this );
                            }
                            else {
                                STOK_GIDEN_SERI_NO_SEC( M_FIS->depo_id, M_FIS_SATIRI->urun_id, &seri_no_list, ENUM_URUN_DEPODA, true, seri_no_fatura_durumu, this );
                            }
                        }
                        else {
                            STOK_GIDEN_SERI_NO_SEC( M_FIS->depo_id, M_FIS_SATIRI->urun_id, &seri_no_list, ENUM_URUN_DEPODA, false, seri_no_fatura_durumu, this );
                        }

                        break;
                    }
                }
                commaEdit_satirdaki_urun_miktari->SET_DOUBLE( seri_no_list.size() );

                QString virgullu_seri_no;
                for (int i = 0; i < seri_no_list.size(); i++) {
                    virgullu_seri_no.append( seri_no_list.at( i ) );
                    if( i NE seri_no_list.size() -1  ) {
                        virgullu_seri_no.append( "," );
                    }
                }

                if( lineEdit_seri_no_list->text() EQ virgullu_seri_no ) {
                    return ADAK_RECORD_UNCHANGED;
                }
                lineEdit_seri_no_list->setText( virgullu_seri_no );

                SET_FOCUS( commaEdit_isk_oncesi_birim_fiyat );

                break;
            }
            case ENUM_PARTI_TAKIBI :
                switch ( fatura_alis_satis_turu ) {

                    case ENUM_ALIS_IRSALIYESI_FATURASI : {
                        int parti_kodu_id =  lineEdit_parti_kodu_id->text().toInt();
                        OPEN_STOK_URUN_PARTI_GIRISI_BATCH( &parti_kodu_id, M_FIS_SATIRI->urun_id, M_FIS->depo_id, this );
                        if ( parti_kodu_id NE -1 ) {
                            lineEdit_parti_kodu_id->setText( QString::number( parti_kodu_id ) );
                        }

                        lineEdit_seri_no_list->setText( STK_GET_PARTI_KODU( parti_kodu_id ) );
                        break;
                    }
                    case ENUM_SATIS_IRSALIYESI_FATURASI :
                    default                             : {
                        int parti_id = STOK_GIDEN_PARTI_NO_SEC( M_FIS->depo_id, M_FIS_SATIRI->urun_id, -1, this );
                        if ( parti_id < 1 ) {
                            return ADAK_RECORD_UNCHANGED;
                        }
                        lineEdit_seri_no_list->setText( STK_GET_PARTI_KODU( parti_id ) );
                        lineEdit_parti_kodu_id->setText( QString::number( parti_id ) );
                        break;
                    }
                }

                SET_FOCUS( commaEdit_satirdaki_urun_miktari );
                break;
        }

        /////////////////////

        URUN_BIRIMI_FIYATLARINI_HESAPLA( p_row_number );

        double vergi_haric_tutar = commaEdit_vergi_haric_tutar->GET_DOUBLE();

        double kdv_orani = comboBox_kdv_orani->currentText().toDouble();
        double otv_orani = comboBox_otv_orani->currentText().toDouble();

        commaEdit_vergi_haric_tutar->SET_DOUBLE ( vergi_haric_tutar );

        double otv_tutari      = ROUND ( commaEdit_vergi_haric_tutar->GET_DOUBLE() * ( otv_orani / 100 )  );
        double otv_dahil_tutar = ROUND ( otv_tutari + commaEdit_vergi_haric_tutar->GET_DOUBLE() );

        commaEdit_otv_dahil_tutar->SET_DOUBLE ( otv_dahil_tutar );
        commaEdit_otv_tutari->SET_DOUBLE      ( otv_tutari      );


        commaEdit_kdv_tutari->SET_DOUBLE      ( ROUND ( otv_dahil_tutar * ( kdv_orani / 100 )  ));
        commaEdit_kdv_dahil_tutar->SET_DOUBLE ( ROUND ( otv_dahil_tutar * ( 1 + ( kdv_orani / 100 ) ) ) );

        // SERI NO ILE TAKIP ISLEMINDEN SONRA ISKONTO SONRASI HESAPLANMASI YAPILMALIDIR.
        TOPLU_ISKONTO_SONRASI_SATIR_DEGERLERINI_HESAPLA ( p_row_number );

        FATURA_SATIR_DEGERLERI_HESAPLAMASI( p_row_number, p_button );

        return ADAK_RECORD_CHANGED;
    }
    else {
        return ADAK_RECORD_UNCHANGED;
    }


    double  kalan_miktar            = 0.0;

    if ( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
        parti_id              = 0;
        kalan_miktar          = satirdaki_urun_miktari - iade_edilen_miktar;
        faturalanan_miktar    = 0;
    }
    else {
        iade_edilen_miktar      = 0;
        kalan_miktar            = satirdaki_urun_miktari - faturalanan_miktar;
    }

    satir_tutari        = ROUND ( ( satir_tutari      / satirdaki_urun_miktari ) * kalan_miktar );
    vergi_haric_tutar   = ROUND ( ( vergi_haric_tutar / satirdaki_urun_miktari ) * kalan_miktar );
    kdv_tutari          = ROUND ( ( kdv_tutari        / satirdaki_urun_miktari ) * kalan_miktar );
    kdv_dahil_tutar     = ROUND ( ( kdv_dahil_tutar   / satirdaki_urun_miktari ) * kalan_miktar );
    otv_tutari          = ROUND ( ( otv_tutari        / satirdaki_urun_miktari ) * kalan_miktar );
    otv_dahil_tutar     = ROUND ( ( otv_dahil_tutar   / satirdaki_urun_miktari ) * kalan_miktar );


    int         stok_takip_sekli        = -1;
    QString     doviz_kodu              = DVZ_GET_DOVIZ_KODU ( doviz_id );
    QString     urun_kodu;
    QString     parti_kodu;

    STK_FILL_URUN_BIRIMLERI_COMBOBOX ( urun_id, comboBox_urun_birimleri, M_STOK_BIRIM_ADLARI );

    lineEdit_urun_adi->setReadOnly(false);

    if ( urun_id > 0 ) {

        STK_URUNLER_STRUCT * URUN = new STK_URUNLER_STRUCT;

        STK_URUNLER_TABLOSUNU_OKU(urun_id , URUN);

        urun_kodu        = URUN->urun_kodu;
        urun_adi         = URUN->urun_adi;
        stok_takip_sekli = URUN->stok_takip_sekli;

        delete URUN;

    }

    if ( parti_id > 0 ) {
        parti_kodu = STK_GET_PARTI_KODU(parti_id);
    }

    lineEdit_urun_id->setText                               ( QVariant ( urun_id ).toString()               );
    lineEdit_urun_adi->setText                              ( urun_adi                                      );
    searchEdit_urun_kodu->SET_TEXT                          ( urun_kodu                                     );
    commaEdit_satirdaki_urun_miktari->SET_DOUBLE            ( kalan_miktar                                  );
    comboBox_urun_birimleri->setEditText                    ( urun_birimi                                   );
    commaEdit_temel_birim_katsayisi->SET_DOUBLE             ( temel_birim_katsayisi                         );
    commaEdit_isk_oncesi_birim_fiyat->SET_DOUBLE            ( isk_oncesi_urun_birim_fiyati                  );
    commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE           ( isk_sonrasi_urun_birim_fiyati                 );
    commaEdit_iskonto_yuzdesi->SET_DOUBLE                   ( iskonto_yuzdesi                               );
    commaEdit_iskonto_tutari->SET_DOUBLE                    ( iskonto_tutari                                );
    commaEdit_satir_tutari->SET_DOUBLE                      ( satir_tutari                                  );
    lineedit_para_birimi->setText                           ( doviz_kodu );
    lineEdit_parti_kodu_id->setText                         ( QVariant ( parti_id ).toString()              );
    lineEdit_stok_takip_sekli->setText                      ( QVariant ( stok_takip_sekli ).toString()      );
    commaEdit_vergi_haric_tutar->SET_DOUBLE                 ( vergi_haric_tutar                             );
    commaEdit_kdv_tutari->SET_DOUBLE                        ( kdv_tutari                                    );
    commaEdit_kdv_dahil_tutar->SET_DOUBLE                   ( kdv_dahil_tutar                               );
    commaEdit_otv_tutari->SET_DOUBLE                        ( otv_tutari                                    );
    commaEdit_otv_dahil_tutar->SET_DOUBLE                   ( otv_dahil_tutar                               );
    comboBox_kdv_orani->setCurrentIndex                     ( comboBox_kdv_orani->findText ( kdv_orani )    );
    comboBox_otv_orani->setCurrentIndex                     ( comboBox_otv_orani->findText ( otv_orani )    );
    comboBox_kdv_dahil_mi->setCurrentIndex                  ( kdv_dahil_mi                                  );
    lineEdit_seri_no_list->setText                          ( parti_kodu                                    );

    commaEdit_t_is_sonrasi_birim_fiyat->SET_DOUBLE          ( t_is_son_birim_fiyati                         );
    commaEdit_t_is_sonrasi_satir_tutari->SET_DOUBLE         ( t_is_son_satir_tutari                         );
    commaEdit_t_is_sonrasi_vergi_haric_tutar->SET_DOUBLE    ( t_is_son_str_vergi_haric_tutar              );
    commaEdit_t_is_sonrasi_otv_tutari->SET_DOUBLE           ( t_is_son_satir_otv_tutari                     );
    commaEdit_t_is_sonrasi_otv_dahil_tutar->SET_DOUBLE      ( t_is_son_satir_otv_dahil_tutar                );
    commaEdit_t_is_sonrasi_kdv_tutari->SET_DOUBLE           ( t_is_son_satir_kdv_tutari                     );
    commaEdit_t_is_sonrasi_kdv_dahil_tutar->SET_DOUBLE      ( t_is_son_satir_kdv_dahil_tutar                );

    /*
        Irsaliye Faturalastirmada serinolu urun secim ekraninda
        Sadece faturalanmayanlarin gelmesi lazim.
     */

    int seri_no_fatura_durumu = -1;
    switch( M_FIS->fatura_turu ) {

        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS            :
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS             :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS   :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS             :
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS              :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS    :
            seri_no_fatura_durumu  = -2;
            break;
        default:
            break;
    }

    QString seri_nolar ;
    if ( lineEdit_stok_takip_sekli->text().toInt() EQ ENUM_SERI_NO_ILE_TAKIP ) {
        if( m_iade_faturasi_mi EQ ENUM_FAT_IADE ) {
            if ( M_FIS->fatura_alis_satis_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI ) {
                // alistan iade
                seri_nolar = STK_GET_SERI_NO_BILGILERI( irsaliye_satiri_id, ENUM_URUN_DEPODA );
            }
            else {
                seri_nolar = STK_GET_SERI_NO_BILGILERI( irsaliye_satiri_id, ENUM_URUN_DEPODA_DEGIL );
            }
        }
        else {
            seri_nolar = STK_GET_SERI_NO_BILGILERI( irsaliye_satiri_id , ENUM_URUN_DEPODA, seri_no_fatura_durumu );
        }
    }

    lineEdit_seri_no_list->setText( seri_nolar );

    URUN_BILGILERINI_AL                                 ( p_row_number );
    TEMEL_BIRIM_CINSINDEN_KATSAYI_HESAPLA               ( p_row_number );

    lineEdit_urun_adi->setReadOnly                      ( true  );
    searchEdit_urun_kodu->setEnabled                    ( false );
    comboBox_urun_birimleri->setEnabled                 ( false );

    if ( lineEdit_stok_takip_sekli->text().toInt() EQ ENUM_BASIT_TAKIP ) {
        pushButton_seri_parti_kodu->setEnabled( false );
    }
    else {
        pushButton_seri_parti_kodu->setEnabled( true );
    }

    //! IRSALIYEDE FIYAT GIRILMIS ISE SADECE MIKTAR, ISKONTO ve ISKONTO ONCESI BIRIM FIYAT UZERINDE DEGISIKLIK YAPABILIR
    if ( isk_sonrasi_urun_birim_fiyati > 0 ) {
        lineEdit_fiyat_irsaliyeden_alindi->setText      ( "1"   );
        //commaEdit_isk_oncesi_birim_fiyat->setEnabled    ( false );
        commaEdit_isk_sonrasi_birim_fiyat->setEnabled   ( false );
        commaEdit_iskonto_yuzdesi->setEnabled           ( true );
        commaEdit_iskonto_tutari->setEnabled            ( false );
        lineedit_para_birimi->setEnabled                ( false );
        commaEdit_vergi_haric_tutar->setEnabled         ( false );
        comboBox_kdv_orani->setEnabled                  ( false );
        comboBox_otv_orani->setEnabled                  ( false );
        comboBox_kdv_dahil_mi->setEnabled               ( false );
    }
    //! IRSALIYEDE FIYAT GIRILMEMIS ISE FUTARADA GIRILMEK ZORUNDA
    else {
        lineEdit_fiyat_irsaliyeden_alindi->clear    ();
        commaEdit_isk_oncesi_birim_fiyat->setEnabled    ( true );
        commaEdit_isk_sonrasi_birim_fiyat->setEnabled   ( true );
        commaEdit_iskonto_yuzdesi->setEnabled           ( true );
        commaEdit_iskonto_tutari->setEnabled            ( true );
        commaEdit_satir_tutari->setEnabled              ( true );
        lineedit_para_birimi->setEnabled                ( true );
        commaEdit_vergi_haric_tutar->setEnabled         ( true );
        comboBox_kdv_orani->setEnabled                  ( true );
        comboBox_otv_orani->setEnabled                  ( true );
        comboBox_kdv_dahil_mi->setEnabled               ( true );
    }

    return ADAK_RECORD_CHANGED;
}

/**************************************************************************************
                   FAT_FATURA_FISI::SECILI_IRSALIYENIN_SATIRLARINI_FATURADAN_SIL
***************************************************************************************/

void FAT_FATURA_FISI::SECILI_IRSALIYENIN_SATIRLARINI_FATURADAN_SIL ( int p_current_row )
{
    int irsaliye_id = m_ui->tableWidget_faturanin_irsaliyeleri->item ( p_current_row, 0 )->text().toInt();
    int fis_id      = m_ui->tableWidget_faturanin_irsaliyeleri->item ( p_current_row, 1 )->text().toInt();


    QList <int> irs_fis_satiri_id_list = STK_GET_FIS_SATIRI_ID_LIST(irsaliye_id);

    if ( irs_fis_satiri_id_list.isEmpty() EQ true ) {
        return;
    }

    DB->START_TRANSACTION();

    for ( int i = 0 ; i < irs_fis_satiri_id_list.size() ; i++ ) {

        int fatura_satiri_id = FAT_GET_FAT_IRS_STR_BAGLANTISI_FATURA_SATIRI_ID(irs_fis_satiri_id_list.at(i));

        FATURA_FIS_SATIRI_STRUCT FIS_SATIRI;

        FATURA_CLEAR_FIS_SATIRI_STRUCT( &FIS_SATIRI);

        FATURA_SATIR_BILGILERINI_OKU( &FIS_SATIRI, fatura_satiri_id );

        FATURA_FIS_SATIRI_SIL( M_FIS, &FIS_SATIRI, fatura_satiri_id );
    }

    STK_UPDATE_IRSALIYE_FATURALASTIRILDI_MI(irsaliye_id , 0);

    m_ui->tableWidget_faturanin_irsaliyeleri->removeRow ( p_current_row );

    SQL_QUERY sql_query(DB);

    if ( m_ui->tableWidget_faturanin_irsaliyeleri->rowCount() EQ 0 ) {

        sql_query.PREPARE_UPDATE ("fat_faturalar" ,"fis_id","fatura_irsaliyelestirildi_mi  ",
                                  "fis_id = :fis_id");
        sql_query.SET_VALUE      (":fatura_irsaliyelestirildi_mi" , 0);
        sql_query.SET_VALUE      (":fis_id", fis_id );
        sql_query.UPDATE         ();
    }

    sql_query.PREPARE_DELETE( "fat_irs_baglantisi","irsaliye_id = :irsaliye_id");
    sql_query.SET_VALUE      (":irsaliye_id" , irsaliye_id   );
    sql_query.DELETE();

    DB->COMMIT_TRANSACTION();

    REFRESH_FIS_RECORD ();
}

/**************************************************************************************
                   FAT_FATURA_FISI::IADE_ISLEMI_YAPILMIS_SATIR_MI
***************************************************************************************/

bool FAT_FATURA_FISI::IADE_ISLEMI_YAPILMIS_SATIR_MI ( int p_fis_satiri_id )
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "fat_fatura_satirlari ","iade_edilen_miktar",
                           "fis_satiri_id = :fis_satiri_id" );
    query.SET_VALUE      ( ":fis_satiri_id", p_fis_satiri_id );

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr("Fatura satır bilgileri okunamadı"), NULL );
        return false;
    }
    query.NEXT();

    if ( query.VALUE(0).toDouble() > 0 ) {
        QMessageBox::warning ( this, tr("HATA"), tr("Bu satırda iade işlemi gerçekleştirilmiş. Satır üzerinde değişiklik veya silme işlemi yapamazsınız.") );
        return false;
    }
    return true;
}



/*****************************************************************************************************
                 FAT_FATURA_FISI::TOPLU_ISKONTO_SONRASI_SATIR_DEGERLERINI_HESAPLA
*******************************************************************************************************/

void FAT_FATURA_FISI::TOPLU_ISKONTO_SONRASI_SATIR_DEGERLERINI_HESAPLA ( int p_row_number )
{
    QCommaEdit  *   commaEdit_iskonto_sonrasi_birim_fiyat     = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN        );
    QCommaEdit  *   commaEdit_satir_tutari                    = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN                   );
    QCommaEdit  *   commaEdit_vergi_haric_tutar               = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTAR_COLUMN              );
    QCommaEdit  *   commaEdit_otv_tutari                      = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_TUTARI_COLUMN                     );
    QCommaEdit  *   commaEdit_otv_dahil_tutar                 = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_DAHIL_TUTAR_COLUMN                );
    QCommaEdit  *   commaEdit_kdv_tutari                      = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_TUTARI_COLUMN                     );
    QCommaEdit  *   commaEdit_kdv_dahil_tutar                 = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_TUTAR_COLUMN                );

    QCommaEdit  *   commaEdit_toplu_iskonto_yuzdesi           = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, TOPLU_ISKONTO_YUZDESI_COLUMN          );
    QCommaEdit  *   commaEdit_t_is_sonrasi_birim_fiyat        = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_BIRIM_FIYAT_COLUMN       );
    QCommaEdit  *   commaEdit_t_is_sonrasi_satir_tutari       = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_SATIR_TUTARI_COLUMN      );
    QCommaEdit  *   commaEdit_t_is_sonrasi_vergi_haric_tutar  = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN );
    QCommaEdit  *   commaEdit_t_is_sonrasi_otv_tutari         = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_OTV_TUTARI_COLUMN        );
    QCommaEdit  *   commaEdit_t_is_sonrasi_otv_dahil_tutar    = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN   );
    QCommaEdit  *   commaEdit_t_is_sonrasi_kdv_tutari         = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_KDV_TUTARI_COLUMN        );
    QCommaEdit  *   commaEdit_t_is_sonrasi_kdv_dahil_tutar    = ( QCommaEdit * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN   );

    double toplu_iskonto_yuzdesi  = m_ui->commaEdit_iskonto_yuzdesi->GET_DOUBLE();

    commaEdit_toplu_iskonto_yuzdesi->SET_DOUBLE             ( toplu_iskonto_yuzdesi );
    commaEdit_t_is_sonrasi_birim_fiyat->SET_DOUBLE          ( commaEdit_iskonto_sonrasi_birim_fiyat->GET_DOUBLE() * ( 1 - ( toplu_iskonto_yuzdesi / 100.00 ) ) );
    commaEdit_t_is_sonrasi_satir_tutari->SET_DOUBLE         ( commaEdit_satir_tutari->GET_DOUBLE()                * ( 1 - ( toplu_iskonto_yuzdesi / 100.00 ) ) );
    commaEdit_t_is_sonrasi_vergi_haric_tutar->SET_DOUBLE    ( commaEdit_vergi_haric_tutar->GET_DOUBLE()           * ( 1 - ( toplu_iskonto_yuzdesi / 100.00 ) ) );
    commaEdit_t_is_sonrasi_otv_tutari->SET_DOUBLE           ( commaEdit_otv_tutari->GET_DOUBLE()                  * ( 1 - ( toplu_iskonto_yuzdesi / 100.00 ) ) );
    commaEdit_t_is_sonrasi_otv_dahil_tutar->SET_DOUBLE      ( commaEdit_otv_dahil_tutar->GET_DOUBLE()             * ( 1 - ( toplu_iskonto_yuzdesi / 100.00 ) ) );
    commaEdit_t_is_sonrasi_kdv_tutari->SET_DOUBLE           ( commaEdit_kdv_tutari->GET_DOUBLE()                  * ( 1 - ( toplu_iskonto_yuzdesi / 100.00 ) ) );
    commaEdit_t_is_sonrasi_kdv_dahil_tutar->SET_DOUBLE      ( commaEdit_kdv_dahil_tutar->GET_DOUBLE()             * ( 1 - ( toplu_iskonto_yuzdesi / 100.00 ) ) );

}

/*****************************************************************************************************
                 FAT_FATURA_FISI::KDV_ORANLARI_COMBOBOX_DOLDUR
*******************************************************************************************************/

void FAT_FATURA_FISI::KDV_ORANLARI_COMBOBOX_DOLDUR ( QComboBox * p_comboBox )
{
    for ( int i = 0; i < M_FIS->kdv_orani_sayisi; i++ ) {
        p_comboBox->addItem ( QVariant ( M_FIS->KDV_ORANLARI_ARRAY[i] ).toString() );
    }
}

/*****************************************************************************************************
                 FAT_FATURA_FISI::OTV_ORANLARI_COMBOBOX_DOLDUR
*******************************************************************************************************/

void FAT_FATURA_FISI::OTV_ORANLARI_COMBOBOX_DOLDUR ( QComboBox * p_comboBox )
{
    for ( int i = 0; i < M_FIS->otv_orani_sayisi; i++ ) {
        p_comboBox->addItem ( QVariant ( M_FIS->OTV_ORANLARI_ARRAY[i] ).toString() );
    }
}

/**************************************************************************************
                   FAT_FATURA_FISI::FATURA_BAGLANTI_BILGISINI_GOSTER
***************************************************************************************/

void FAT_FATURA_FISI::FATURA_BAGLANTI_BILGISINI_GOSTER( int p_fatura_turu, int p_fis_id )
{

    SQL_QUERY query( DB );

    switch ( p_fatura_turu ) {

        case ENUM_FAT_FATURA_ARTI_IRSALIYE_SATIS                  :
        case ENUM_FAT_FATURA_ARTI_IRSALIYE_ALIS                   :
        case ENUM_FAT_IRSALIYELI_ALIS_FATURASI                    :
        case ENUM_FAT_IRSALIYELI_SATIS_FATURASI                   :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_ALIS  :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_KONSINYE_SATIS :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS           :
        case ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS          :
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_ALIS            :
        case ENUM_FAT_OTO_IRSALIYE_FATURALASTIRMA_SATIS           : {

            query.PREPARE_SELECT("fat_irs_baglantisi", "irsaliye_id ", "fatura_id  = :fis_id");
            query.SET_VALUE(":fis_id", p_fis_id );

            if ( query.SELECT() EQ 0 ) {
                break;
            }

            m_ui->textEdit_irsaliye_bilgisi->setVisible( true );
            m_ui->textEdit_irsaliye_bilgisi->setText( FAT_IRS_BAGLANTISI_BILGISI( p_fis_id ));

        }
            break;
        default:
            break;
    }

}

/**************************************************************************************
                   FAT_FATURA_FISI::CARI_BORC_BILGISINI_GOSTER
***************************************************************************************/

void FAT_FATURA_FISI::CARI_BORC_BILGISINI_GOSTER(int p_cari_hesap_id)
{

    if( p_cari_hesap_id < 1 ) {
        return;
    }

    m_ui->commaEdit_bors_alacak_durumu->setVisible( true );
    m_ui->label_cari_borc_alacak->setVisible( true );
    m_ui->lineEdit_cari_para_birimi->setVisible( true );

    BORC_BILGISI borc_bilgisi;


    CARI_KART_BORC_BILGISINI_GETIR( p_cari_hesap_id, &borc_bilgisi );

    if ( borc_bilgisi.borc_bakiye > borc_bilgisi.alacak_bakiye ) {
        m_ui->label_cari_borc_alacak->setText( "Borç :");
        m_ui->commaEdit_bors_alacak_durumu->SET_DOUBLE( borc_bilgisi.borc_bakiye );
    }
    else {
        m_ui->label_cari_borc_alacak->setText( "Alacak :");
        m_ui->commaEdit_bors_alacak_durumu->SET_DOUBLE( borc_bilgisi.alacak_bakiye );
    }

    m_ui->label_cari_borc_alacak->setAlignment( Qt::AlignRight );
    m_ui->lineEdit_cari_para_birimi->setText( DVZ_GET_DOVIZ_KODU( M_FIS->cari_para_birimi_id ) );
}


/*****************************************************************************************************
                 FAT_FATURA_FISI::FAT_KUR_VE_PARITE_AYARLA
*******************************************************************************************************/

void FAT_FATURA_FISI::FAT_KUR_VE_PARITE_AYARLA( bool p_degerleri_oto_ata )
{
    bool cari_fis_tutarini_goster =  true;
    m_ui->label_temel_para_birimi->setText( m_temel_para_birimi_kodu );

    //! ortak olan durumlar
    m_ui->label_fatura_para_birimi_1->setVisible( true );
    m_ui->label_fatura_para_birimi_1->setText( "Kur  : 1 "+ M_FIS->fatura_para_birimi_kodu );
    m_ui->commaEdit_kur->setVisible( true );
    m_ui->label_temel_para_birimi->setVisible( true );
    m_ui->label_parite->setVisible( false );
    m_ui->commaEdit_parite->setVisible( false );
    m_ui->label_cari_para_birimi->setVisible( false );
    m_ui->label_fatura_para_birimi_2->setVisible( false );
    m_ui->commaEdit_parite->SET_DOUBLE( 1.000 );
    //! end ortak olan durum

    if ( M_FIS->cari_para_birimi_id EQ m_temel_para_birimi_id ) {

        if ( M_FIS->cari_para_birimi_id EQ M_FIS->fatura_para_birimi_id ) {
            //hepsi temel para birimi
            m_ui->commaEdit_kur->setVisible( false );
            m_ui->label_temel_para_birimi->setVisible( false );
            m_ui->label_parite->setVisible( false );
            m_ui->commaEdit_parite->setVisible( false );
            m_ui->label_cari_para_birimi->setVisible( false );
            m_ui->label_fatura_para_birimi_1->setVisible( false );
            m_ui->label_fatura_para_birimi_2->setVisible( false );
            m_ui->commaEdit_kur->SET_DOUBLE( 1.000 );
            m_ui->commaEdit_parite->SET_DOUBLE( 1.000 );

            cari_fis_tutarini_goster = false;
        }
        else {
            // Irsaliye para birimi farkli
            if( p_degerleri_oto_ata EQ true ) {
                m_ui->commaEdit_kur->SET_DOUBLE( DVZ_PARA_BIRIMLERINI_DONUSTUR ( M_FIS->fatura_para_birimi_id, 1.00, m_temel_para_birimi_id, m_ui->adakDate_fatura_tarihi->QDATE(), ENUM_SATIS ) );
            }
        }
    }
    else {
        if( M_FIS->cari_para_birimi_id EQ M_FIS->fatura_para_birimi_id ) {
            // irsaliye ile cari ayni temelden farklilar
            if( p_degerleri_oto_ata EQ true ) {
                m_ui->commaEdit_kur->SET_DOUBLE( DVZ_PARA_BIRIMLERINI_DONUSTUR ( M_FIS->cari_para_birimi_id, 1.00, m_temel_para_birimi_id, m_ui->adakDate_fatura_tarihi->QDATE(), ENUM_SATIS ) );
            }

            cari_fis_tutarini_goster = false;
        }
        else {

            if( M_FIS->fatura_para_birimi_id EQ m_temel_para_birimi_id ) {
                // cari farkli digerleri ayni
                if( p_degerleri_oto_ata EQ true ) {
                    m_ui->commaEdit_kur->SET_DOUBLE( DVZ_PARA_BIRIMLERINI_DONUSTUR ( m_temel_para_birimi_id, 1.00, M_FIS->cari_para_birimi_id, m_ui->adakDate_fatura_tarihi->QDATE(), ENUM_SATIS ) );
                }
                m_ui->label_temel_para_birimi->setVisible( true );
                m_ui->label_temel_para_birimi->setText( DVZ_GET_DOVIZ_KODU( M_FIS->cari_para_birimi_id ));
            }
            else {
                // hepsi biribirinden farkli
                if( p_degerleri_oto_ata EQ true ) {
                    m_ui->commaEdit_kur->SET_DOUBLE( DVZ_DOVIZ_KURUNU_AL( M_FIS->fatura_para_birimi_id, m_ui->adakDate_fatura_tarihi->QDATE(), ENUM_SATIS ) );
                    m_ui->commaEdit_parite->SET_DOUBLE( DVZ_PARITEYI_AL( M_FIS->cari_para_birimi_id, M_FIS->fatura_para_birimi_id, m_ui->adakDate_fatura_tarihi->QDATE(), ENUM_SATIS ) );
                }

                m_ui->label_parite->setVisible( true );
                m_ui->commaEdit_parite->setVisible( true );
                m_ui->label_cari_para_birimi->setVisible( true );
                m_ui->label_fatura_para_birimi_2->setText( "1 " + DVZ_GET_DOVIZ_KODU( M_FIS->fatura_para_birimi_id ) );
                m_ui->label_cari_para_birimi->setText( DVZ_GET_DOVIZ_KODU( M_FIS->cari_para_birimi_id ) );
                m_ui->label_fatura_para_birimi_2->setVisible( true );
            }
        }
    }

    if( cari_fis_tutarini_goster EQ true ) {
        m_ui->commaEdit_cari_para_birimi_tutari->setVisible( true );
        m_ui->label_cari_para_birimi_tutari->setVisible( true );
    }
}

/**************************************************************************************
                   FAT_FATURA_FISI::FAT_FIS_TOPLAMLARINI_YENILE
***************************************************************************************/

void FAT_FATURA_FISI::FAT_FIS_TOPLAMLARINI_YENILE(FATURA_FISI_STRUCT *P_FIS )
{
    m_ui->commaEdit_brut_tutar->SET_DOUBLE                  ( P_FIS->brut_tutar                  );
    m_ui->commaEdit_ara_toplam->SET_DOUBLE                  ( P_FIS->ara_toplam                  );
    m_ui->commaEdit_isk_sonrasi_kdv_tutari->SET_DOUBLE      ( P_FIS->isk_sonrasi_kdv_tutari      );
    m_ui->commaEdit_fatura_tutari->SET_DOUBLE               ( P_FIS->fatura_tutari               );
    m_ui->commaEdit_isk_sonrasi_otv_tutari->SET_DOUBLE      ( P_FIS->isk_sonrasi_otv_tutari      );
    m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->SET_DOUBLE ( P_FIS->isk_sonrasi_otv_dahil_tutar );
    m_ui->commaEdit_iskonto_tutari->SET_DOUBLE              ( P_FIS->iskonto_tutari              );
    m_ui->commaEdit_cari_para_birimi_tutari->SET_DOUBLE     ( P_FIS->cari_para_birimi_tutari     );

}

/**************************************************************************************
                   FAT_FATURA_FISI::FATURA_SATIR_DEGERLERI_HESAPLAMASI
***************************************************************************************/

int FAT_FATURA_FISI::FATURA_SATIR_DEGERLERI_HESAPLAMASI(int p_row_number, QObject *p_object)
{

    QCommaEdit  * commaEdit_iskonto_oncesi_birim_fiyat      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN    );
    QCommaEdit  * commaEdit_iskonto_sonrasi_birim_fiyat     = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN   );
    QCommaEdit  * commaEdit_iskonto_yuzdesi                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_YUZDESI_COLUMN           );
    QCommaEdit  * commaEdit_iskonto_tutari                  = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, ISKONTO_TUTARI_COLUMN            );
    QComboBox   * comboBox_urun_birimleri                   = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN               );
    QPushButton * pushButton_seri_parti_kodu                = ( QPushButton * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SERI_PARTI_NO_KODU_COLUMN        );
    QCommaEdit  * commaEdit_satirdaki_urun_miktari          = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIRDAKI_URUN_MIKTARI_COLUMN    );
    QCommaEdit  * commaEdit_satir_tutari                    = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN              );
    QComboBox   * comboBox_kdv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_ORANI_COLUMN                 );
    QComboBox   * comboBox_kdv_dahil_mi                     = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN              );
    QCommaEdit  * commaEdit_kdv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_DAHIL_TUTAR_COLUMN           );
    QCommaEdit  * commaEdit_kdv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, KDV_TUTARI_COLUMN                );
    QComboBox   * comboBox_otv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_ORANI_COLUMN                 );
    QCommaEdit  * commaEdit_vergi_haric_tutar               = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTAR_COLUMN         );
    QCommaEdit  * commaEdit_otv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_DAHIL_TUTAR_COLUMN           );
    QCommaEdit  * commaEdit_otv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, OTV_TUTARI_COLUMN                );
    QLineEdit   * lineEdit_makbuz_para_birimi               = ( QLineEdit   * ) m_ui->tableWidget_fatura_satirlari->cellWidget ( p_row_number, FATURA_PARA_BIRIMI_COLUMN            );

    if ( ( p_object EQ comboBox_urun_birimleri) OR ( p_object EQ commaEdit_satirdaki_urun_miktari )
           OR ( p_object EQ commaEdit_satir_tutari ) OR ( p_object EQ commaEdit_iskonto_oncesi_birim_fiyat )
           OR ( p_object EQ commaEdit_iskonto_sonrasi_birim_fiyat ) OR ( p_object EQ commaEdit_iskonto_yuzdesi )
           OR ( p_object EQ lineEdit_makbuz_para_birimi ) OR ( p_object EQ comboBox_kdv_orani )
           OR ( p_object EQ comboBox_kdv_dahil_mi ) OR ( p_object EQ comboBox_otv_orani )
           OR ( p_object EQ pushButton_seri_parti_kodu ) ) {

        //! EGER IRSALIYE FATURALASTIRILIYOR ISE SATIRDAKI URUN IRSALIYEDEKI URUNDEN FAZLA OLAMAZ

        if( p_object EQ commaEdit_satirdaki_urun_miktari ) {
            if ( m_fatura_turu EQ ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_ALIS OR
                 m_fatura_turu EQ ENUM_FAT_ELLE_IRSALIYE_FATURALASTIRMA_SATIS ) {
                if ( commaEdit_satirdaki_urun_miktari->GET_DOUBLE() > m_irs_faturalastirma_str_max_urun_sayi ) {
                    MSG_WARNING( tr( "Faturalanacak Ürün miktarı İrsaliyedeki ürün miktarı büyük olamaz..." ), commaEdit_satirdaki_urun_miktari );
                    return ADAK_FAIL_UNDO;
                }
            }
        }
        if ( p_object EQ commaEdit_satir_tutari ) {
            if ( commaEdit_satirdaki_urun_miktari->GET_DOUBLE() EQ 0 ) {
                commaEdit_satirdaki_urun_miktari->SET_DOUBLE ( 1.00 );
                commaEdit_iskonto_oncesi_birim_fiyat->SET_DOUBLE ( commaEdit_satir_tutari->GET_DOUBLE() );
            }
        }

        if ( p_object EQ commaEdit_iskonto_yuzdesi ) {
            if ( commaEdit_iskonto_yuzdesi->GET_DOUBLE() > 100 ) {
                QMessageBox::warning ( this, tr("HATA"), tr("İskonto yüzdesi 100 den büyük olamaz.") );
                SET_FOCUS( commaEdit_iskonto_yuzdesi );
                return ADAK_FAIL_UNDO;
            }
        }

        URUN_BILGILERINI_AL                   ( p_row_number );
        TEMEL_BIRIM_CINSINDEN_KATSAYI_HESAPLA ( p_row_number );


        if ( p_object EQ comboBox_urun_birimleri ) {
            URUN_BIRIMI_FIYATLARINI_HESAPLA ( p_row_number );
        }

        if ( p_object EQ commaEdit_iskonto_sonrasi_birim_fiyat ) {
            commaEdit_iskonto_tutari->SET_DOUBLE  ( commaEdit_iskonto_oncesi_birim_fiyat->GET_DOUBLE() - commaEdit_iskonto_sonrasi_birim_fiyat->GET_DOUBLE() );
            commaEdit_iskonto_yuzdesi->SET_DOUBLE ( ( ( commaEdit_iskonto_oncesi_birim_fiyat->GET_DOUBLE() - commaEdit_iskonto_sonrasi_birim_fiyat->GET_DOUBLE() ) / commaEdit_iskonto_oncesi_birim_fiyat->GET_DOUBLE() ) * 100 );
        }
        else if ( p_object EQ commaEdit_iskonto_yuzdesi OR p_object EQ commaEdit_iskonto_oncesi_birim_fiyat ) {
            double iskonto_yuzdesi = commaEdit_iskonto_yuzdesi->GET_DOUBLE();
            commaEdit_iskonto_sonrasi_birim_fiyat->SET_DOUBLE ( commaEdit_iskonto_oncesi_birim_fiyat->GET_DOUBLE() * ( 1 - ( iskonto_yuzdesi / 100.0 ) ) );
            commaEdit_iskonto_tutari->SET_DOUBLE ( commaEdit_iskonto_oncesi_birim_fiyat->GET_DOUBLE() - commaEdit_iskonto_sonrasi_birim_fiyat->GET_DOUBLE() );
        }

        // ISKONTO SONRASI VE ONCESI TUTARLAR TEKRAR HESAPLANMIS OLABILIRLER

        double iskonto_sonrasi_birim_fiyat  = commaEdit_iskonto_sonrasi_birim_fiyat->GET_DOUBLE();
        double satir_stok_miktari           = commaEdit_satirdaki_urun_miktari->GET_DOUBLE();
        double satir_tutari                 = commaEdit_satir_tutari->GET_DOUBLE();

        double kdv_orani          = comboBox_kdv_orani->currentText().toDouble();
        double otv_orani          = comboBox_otv_orani->currentText().toDouble();
        double kdv_tutari         = 0.00;
        double kdv_dahil_tutar    = 0.00;
        double vergi_haric_tutar  = 0.00;
        double otv_tutari         = 0.00;

        //! [1] ayni hesaplama irsaliyede var
        if ( p_object NE commaEdit_satir_tutari ) {
            satir_tutari = ROUND( satir_stok_miktari * iskonto_sonrasi_birim_fiyat );
        }

        if ( p_object EQ commaEdit_satir_tutari ) {
            if ( comboBox_kdv_dahil_mi->currentIndex() EQ 1 ) {
                vergi_haric_tutar   = ROUND ( commaEdit_satir_tutari->GET_DOUBLE() / ( 1 + ( kdv_orani / 100.0 ) ) );
            }
            else {
                vergi_haric_tutar = satir_tutari;
            }
        }
        else {
            if ( comboBox_kdv_dahil_mi->currentIndex() EQ 1 ) {
                vergi_haric_tutar   = ROUND ( satir_tutari / ( 1 + ( kdv_orani / 100.0 ) ) );
            }
            else {
                vergi_haric_tutar = satir_tutari;
            }
        }

        otv_tutari      = ROUND ( vergi_haric_tutar * ( otv_orani / 100.0 ));
        kdv_tutari      = ROUND ( vergi_haric_tutar * ( kdv_orani / 100.0 ) );
        kdv_dahil_tutar = ROUND ( vergi_haric_tutar + otv_tutari + kdv_tutari );

        if ( p_object NE commaEdit_satir_tutari ) {
            commaEdit_satir_tutari->SET_DOUBLE    ( vergi_haric_tutar );
        }

        if ( p_object EQ commaEdit_satir_tutari ) {
            if ( comboBox_kdv_dahil_mi->currentIndex() EQ 0 ) {
                commaEdit_iskonto_sonrasi_birim_fiyat->SET_DOUBLE ( ROUND ( vergi_haric_tutar / satir_stok_miktari ) );
            }
            else {
                commaEdit_iskonto_sonrasi_birim_fiyat->SET_DOUBLE ( ROUND ( kdv_dahil_tutar / satir_stok_miktari ) );
            }
        }
        //! [/2]

        double makbuz_para_birimi_vergi_haric_tutar =  commaEdit_satir_tutari->GET_DOUBLE();


        double makbuz_para_birimi_otv_tutari      = ROUND ( makbuz_para_birimi_vergi_haric_tutar * ( otv_orani / 100.0 ) );
        double makbuz_para_birimi_otv_dahil_tutar = ROUND ( makbuz_para_birimi_vergi_haric_tutar * ( 1 + ( otv_orani / 100.0 ) ) );

        double makbuz_para_birimi_kdv_tutari      = ROUND ( makbuz_para_birimi_otv_dahil_tutar * ( kdv_orani / 100.0 ) );
        double makbuz_para_birimi_kdv_dahil_tutar = ROUND ( makbuz_para_birimi_otv_dahil_tutar + makbuz_para_birimi_kdv_tutari );

        commaEdit_vergi_haric_tutar->SET_DOUBLE   ( makbuz_para_birimi_vergi_haric_tutar    );
        commaEdit_otv_tutari->SET_DOUBLE          ( makbuz_para_birimi_otv_tutari           );
        commaEdit_otv_dahil_tutar->SET_DOUBLE     ( makbuz_para_birimi_otv_dahil_tutar      );
        commaEdit_kdv_tutari->SET_DOUBLE          ( makbuz_para_birimi_kdv_tutari           );
        commaEdit_kdv_dahil_tutar->SET_DOUBLE     ( makbuz_para_birimi_kdv_dahil_tutar      );

        TOPLU_ISKONTO_SONRASI_SATIR_DEGERLERINI_HESAPLA ( p_row_number );

    }

    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_FISI::SATIR_PARA_BIRIMINI_GUNCELLE
***************************************************************************************/

void FAT_FATURA_FISI::SATIR_PARA_BIRIMINI_GUNCELLE()
{
    CLOSE_ALL_EVENTS();

    for ( int row = 0 ; row < m_ui->tableWidget_fatura_satirlari->rowCount() ; row++) {

        QLineEdit  * lineedit_makbuz_para_birimi = ( QLineEdit  * ) m_ui->tableWidget_fatura_satirlari->cellWidget( row, FATURA_PARA_BIRIMI_COLUMN );

        lineedit_makbuz_para_birimi->setText( m_ui->comboBox_fatura_para_birimi_id->currentText() );

    }
    OPEN_ALL_EVENTS();
}

