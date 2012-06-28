#include <QDate>
#include "adak_sql.h"
#include "adak_utils.h"
#include "adak_std_utils.h"
#include "ui_stok_fisi.h"
#include "stok_fisi_class.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "dvz_gui_utils.h"
#include "muh_console_utils.h"
#include "cari_console_utils.h"
#include "sube_console_utils.h"
#include "fat_struct.h"
#include "fat_enum.h"
#include "irs_enum.h"
#include "e9_log.h"
#include "e9_yetki.h"
#include "stok_fis_utils.h"
#include "muh_fis_utils.h"
#include "ui_stok_fisi.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "fat_console_utils.h"
#include "fat_fis_utils.h"
#include "cari_kart_arama_open.h"
#include "irs_fatura_satir_secimi_open.h"
#include "irs_fatura_secimi_open.h"
#include "irs_iade_irsaliye_secimi_open.h"
#include "irs_iade_satir_secimi_open.h"
#include "stok_open.h"
#include "sube_depo_secimi_open.h"
#include "irs_irsaliye_belgesi_open.h"
#include "irs_console_utils.h"
#include "irs_gui_utils.h"
#include "muh_enum.h"
#include "sube_struct.h"
#include "sube_enum.h"
#include "stok_enum.h"
#include "e9_enum.h"
#include "stok_struct.h"
#include "cari_struct.h"
#include "stok_console_utils.h"
#include "stok_gui_utils.h"
#include "stok_gelen_seri_no_batch_open.h"
#include "stok_giden_seri_no_secimi_open.h"
#include "stok_urun_parti_girisi_batch_open.h"
#include "stok_giden_parti_no_secimi_open.h"
#include "sube_console_utils.h"
#include "e9_uretilen_fis_secimi_open.h"
#include "cari_oto_ekstre_arama_open.h"

#define     VERGI_ORANI_EKLE                     100
#define     VERGI_ORANI_SIL                      200


#define     ROW_ID_COLUMN                           0
#define     FIS_ID_COLUMN                           1
#define     ORDER_NUM_COLUMN                        2
#define     URUN_ID_COLUMN                          3
#define     STOK_TAKIP_SEKLI_COLUMN                 4
#define     PARTI_ID_COLUMN                         5
#define     FATURA_SATIRI_ID_COLUMN                 6
#define     IADE_EDILEN_IRS_SATIR_ID_COLUMN         7
#define     HATALI_SATIR_MI_COLUMN                  8
#define     TEMEL_BIRIM_KATSAYISI                   9
#define     SERI_NO_LISTE_COLUMN                    10
#define     FATURA_SATIRI_SECIMI_COLUMN             11
#define     IADE_IRSALIYE_SATIRI_SECIMI_COLUMN      12
#define     URUN_KOD_COLUMN                         13
#define     URUN_AD_COLUMN                          14
#define     SERI_NO_PARTI_KODU_COLUMN               15
#define     STOK_MIKTARI_COLUMN                     16
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
#define     VERGI_HARIC_TUTARI_COLUMN               34
#define     PARA_BIRIMI_COLUMN                      35
#define     KDV_TUTAR_COLUMN                        36
#define     KDV_DAHIL_TUTARI_COLUMN                 37
#define     OTV_TUTARI_COLUMN                       38
#define     OTV_DAHIL_TUTAR_COLUMN                  39

extern ADAK_SQL *              DB;


/**************************************************************************************
                   OPEN_STOK_FISI
***************************************************************************************/

void OPEN_STOK_FISI ( int p_fis_id, int p_irsaliye_mi_stok_fisi_mi, int p_fis_turu, int p_iade_irsaliyesi_mi, QWidget * p_parent )
{
    if ( SUBE_DEPO_ISLEM_YETKI_KONTROLU ( p_parent) EQ false ) {
        return;
    }

    STOK_FISI * F = new STOK_FISI ( p_fis_id, p_irsaliye_mi_stok_fisi_mi, p_fis_turu, p_iade_irsaliyesi_mi, p_parent );
    F->SHOW( FULL_SCREEN );
}

/**************************************************************************************
                   STOK_FISI::STOK_FISI
***************************************************************************************/

STOK_FISI::STOK_FISI ( int p_fis_id, int p_irsaliye_mi_stok_fisi_mi, int p_fis_turu, int p_iade_irsaliyesi_mi, QWidget * p_parent ) :FIS_KERNEL ( p_parent ), m_ui ( new Ui::STOK_FISI )
{
    M_FIS = new STK_IRS_FIS_STRUCT;
    M_FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

    STK_IRS_CLEAR_FIS_STRUCT( M_FIS );
    STK_IRS_CLEAR_SATIR_STRUCT( M_FIS_SATIRI );

    M_FIS->irsaliye_mi_stok_fisi_mi = p_irsaliye_mi_stok_fisi_mi;
    M_FIS->fis_turu             = p_fis_turu;
    M_FIS->iade_irsaliyesi_mi   = p_iade_irsaliyesi_mi;

    m_fis_id = p_fis_id;

    m_ui->setupUi       ( this );
    START_FIS_KERNEL    ( this, DB );
 }

/**************************************************************************************
                   STOK_FISI::SETUP_FORM
***************************************************************************************/

void STOK_FISI::SETUP_FORM()
{
    SET_HELP_PAGE ( "irsaliye-islemleri" );

    REGISTER_BUTTONS_WIDGET( m_ui->navigation_button_widget );

    //! Otomatik irsaliye islemleri icin irsaliye turu disable ediliyor
    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {
        switch ( M_FIS->fis_turu ) {

        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS  :
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS:
        case ENUM_IRS_FASON_BASLANGIC_GIRIS             :
        case ENUM_IRS_FASON_BASLANGIC_CIKIS             :
        case ENUM_IRS_FASON_BITIS_GIRIS                 :
        case ENUM_IRS_FASON_BITIS_CIKIS                 :
            m_ui->comboBox_irsaliye_turu->setEnabled( false );
            break;
        default:
            break;
        }

        switch ( M_FIS->fis_turu ) {
            case ENUM_IRS_FASON_BASLANGIC_GIRIS             :
            case ENUM_IRS_FASON_BASLANGIC_CIKIS             :
            case ENUM_IRS_FASON_BITIS_GIRIS                 :
            case ENUM_IRS_FASON_BITIS_CIKIS                 :
                m_ui->comboBox_stk_irs_para_birimi_id->setVisible( false );
                m_ui->label_stk_irs_para_birimi_id->setVisible( false );
                break;
            default:
                break;
        }
    }
    //! end

    if ( M_FIS->irsaliye_mi_stok_fisi_mi  EQ ENUM_STOK_FISI ) {
        m_ui->comboBox_irsaliye_turu->setEnabled( false );
    }
    else {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }

    m_siradaki_belge_no.clear();

    m_ui->lineEdit_fis_no->setText("*");

    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_fis_no);
    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_sirket);
    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_stk_irs_para_birimi_id );

    m_temel_para_birimi_kodu     = DVZ_GET_TEMEL_PARA_BIRIMI_KODU ();
    m_temel_para_birimi_id       = DVZ_GET_TEMEL_PARA_BIRIMI_ID   ();

    DVZ_FILL_COMBOBOX( m_ui->comboBox_stk_irs_para_birimi_id );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_stk_irs_para_birimi_id, m_temel_para_birimi_kodu );

    M_STOK_DETAYLARI = new STOK_DETAYLAR_STRUCT;

    REGISTER_SAVER_BUTTON( m_ui->pushButton_urettigi_fisler );
    if ( E9_KULLANICI_FISIN_URETTIGI_ENT_FISLERINI_GOREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }

    m_ui->label_parite->setVisible( false );
    m_ui->commaEdit_parite->setVisible( false );
    m_ui->commaEdit_kur->setVisible( false );

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ HAYIR ) {
        m_ui->comboBox_fis_filtresi->setVisible( false );
        m_ui->label_fis_filtresi->setVisible( false );

        m_ui->comboBox_sirket->setVisible( false );
        m_ui->label_sirket->setVisible( false );
    }

    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_fis_filtresi );

    m_ui->textEdit_satir_tutar_bilgileri->clear();
    m_ui->textEdit_kdv_yuzdeleri->clear();
    m_ui->textEdit_fatura_bilgisi->setHidden ( true );

    m_ui->commaEdit_iade_edilecek_fis_no->SET_PRECISION     ( 0 );
    m_ui->commaEdit_iade_edilecek_fis_no->VIRGULSUZ_CALIS   ( true );

    m_ui->tableWidget_irsaliyenin_faturalari->hideColumn ( 0 );
    m_ui->tableWidget_irsaliyenin_faturalari->hideColumn ( 1 );

    m_log_kaydi_eklenecek_mi = false;
    m_kayit_eklendi          = false;
    m_kayit_silindi          = false;

    m_ui->lineEdit_vergi_no->setMaxLength( 15 );

    M_SUBE_ENT_HESAPLARI = ( SUBE_ENTEGRASYON_HESAPLARI_STRUCT * ) SUBE_GET_ENTEGRASYON_HESAPLARI_STRUCT();

    M_STOK_DETAYLARI->muh_hesap_id  = M_SUBE_ENT_HESAPLARI->sarf_malzemeleri_hesap_id;

    m_ui->label_bilgilendirme->setHidden(true);

    m_ui->lineEdit_irsaliye_belge_seri->setMaxLength(3);

    SUBE_UNITE_BILGILERI_STRUCT UNITE_BILG;

    SUBE_UNITE_BILGILERI_OKU( &UNITE_BILG, SUBE_GET_UNITE_ID() );

    m_irsaliye_belge_seri = UNITE_BILG.irsaliye_belge_seri;

    m_irs_iade_edilecek_gun_sayisi = 0;
    m_ontanimli_kdv_orani = 0;

    SQL_QUERY query( DB );
    query.PREPARE_SELECT ( "e9_sabit_degerler","irs_faturalandirilacak_gun,ontanimli_kdv_orani",
                           "sabit_deger_id = 1");

    query.SELECT();
    query.NEXT();
    m_irs_iade_edilecek_gun_sayisi = query.VALUE(0).toInt();
    m_ontanimli_kdv_orani = query.VALUE(1).toDouble();

    DISABLE_CLEAR_ON_WIDGET ( m_ui->comboBox_irsaliye_turu );

    M_STOK_BIRIM_ADLARI          =  new QString [URUN_BIRIMLERI_SAYISI];
    M_STOK_BIRIM_KATSAYILARI     =  new double  [URUN_BIRIMLERI_SAYISI];
    M_STOK_CAPRAZ_BIRIMLER       =  new int     [URUN_BIRIMLERI_SAYISI];
    M_STOK_BIRIM_ALIS_FIYATLARI  =  new double  [URUN_BIRIMLERI_SAYISI];
    M_STOK_BIRIM_SATIS_FIYATLARI =  new double  [URUN_BIRIMLERI_SAYISI];


    m_ui->commaEdit_parite->SET_PRECISION(6);
    m_ui->commaEdit_kur->SET_PRECISION(4);

    m_last_urun_id               = -1;

    m_depo_islem_yetkisi         =  SUBE_GET_DEPO_ISLEM_YETKISI();


    QRegExp rx ( "(\\*?)(\\d+)" );
    QRegExpValidator * validator = new QRegExpValidator ( rx, this );
    m_ui->lineEdit_fis_no->setValidator(validator);

    int num_of_COLUMNs    = 40;

    REGISTER_TABLE_WIDGET           ( m_ui->tableWidget_fis_satirlari, num_of_COLUMNs );
    SET_FIS_ORDER_COLUMN_NUMBER     ( ORDER_NUM_COLUMN );
    SET_FORM_ID_COLUMN_POSITION     ( FIS_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION      ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN     ( 11 );

    REGISTER_SAVER_BUTTON           ( m_ui->button_fatura_secimi );
    REGISTER_SAVER_BUTTON           ( m_ui->button_yazdir        );
    REGISTER_CHANGER_BUTTON         ( m_ui->button_iade_edilecek_irsaliye_secimi );
    REGISTER_SAVER_BUTTON           ( m_ui->pushButton_detaylar  );

    REGISTER_BUTTON_SHORT_CUT       ( SERI_NO_PARTI_KODU_COLUMN, QKeySequence ( Qt::CTRL + Qt::Key_N ) );

    SET_FIS_ORDER_DATABASE          ( "stk_fis_satirlari", "order_number", "fis_satiri_id" );

    SET_TABLE_ROW_WIDGETS ( URUN_ID_COLUMN                           , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS ( STOK_TAKIP_SEKLI_COLUMN                  , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS ( PARTI_ID_COLUMN                          , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS ( FATURA_SATIRI_ID_COLUMN                  , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS ( IADE_EDILEN_IRS_SATIR_ID_COLUMN          , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS ( HATALI_SATIR_MI_COLUMN                   , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS ( SERI_NO_LISTE_COLUMN                      , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS ( FATURA_SATIRI_SECIMI_COLUMN              , WIDGET_PUSH_BUTTON        );
    SET_TABLE_ROW_WIDGETS ( IADE_IRSALIYE_SATIRI_SECIMI_COLUMN       , WIDGET_PUSH_BUTTON        );
    SET_TABLE_ROW_WIDGETS ( URUN_KOD_COLUMN                          , WIDGET_SEARCH_EDIT        );
    SET_TABLE_ROW_WIDGETS ( URUN_AD_COLUMN                           , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS ( STOK_MIKTARI_COLUMN                      , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( TEMEL_BIRIM_KATSAYISI                    , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( URUN_BIRIMI_COLUMN                       , WIDGET_COMBO_BOX_EDITABLE );
    SET_TABLE_ROW_WIDGETS ( SERI_NO_PARTI_KODU_COLUMN                , WIDGET_PUSH_BUTTON        );
    SET_TABLE_ROW_WIDGETS ( ISK_ONCESI_BIRIM_FIYAT_COLUMN            , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( ISK_SONRASI_BIRIM_FIYAT_COLUMN           , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( ISKONTO_YUZDESI_COLUMN                   , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( ISKONTO_TUTARI_COLUMN                    , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( KDV_DAHIL_MI_COLUMN                      , WIDGET_COMBO_BOX          );
    SET_TABLE_ROW_WIDGETS ( KDV_ORANI_COLUMN                         , WIDGET_COMBO_BOX          );
    SET_TABLE_ROW_WIDGETS ( SATIR_TUTARI_COLUMN                      , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( TOPLU_ISKONTO_YUZDESI_COLUMN             , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( T_IS_SONRASI_BIRIM_FIYAT_COLUMN          , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( T_IS_SONRASI_SATIR_TUTARI_COLUMN         , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN    , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( T_IS_SONRASI_OTV_TUTARI_COLUMN           , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN      , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( T_IS_SONRASI_KDV_TUTARI_COLUMN           , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN      , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( PARA_BIRIMI_COLUMN                       , WIDGET_LINE_EDIT          );
    SET_TABLE_ROW_WIDGETS ( VERGI_HARIC_TUTARI_COLUMN                , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( KDV_TUTAR_COLUMN                         , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( KDV_DAHIL_TUTARI_COLUMN                  , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( OTV_ORANI_COLUMN                         , WIDGET_COMBO_BOX          );
    SET_TABLE_ROW_WIDGETS ( OTV_TUTARI_COLUMN                        , WIDGET_COMMA_EDIT         );
    SET_TABLE_ROW_WIDGETS ( OTV_DAHIL_TUTAR_COLUMN                   , WIDGET_COMMA_EDIT         );


    SET_FIRST_FOCUS_WIDGET       ( m_ui->lineEdit_fis_no );

    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_vergi_dairesi);

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH   ( 512 );
    m_ui->limitedTextEdit_adres->SET_MAX_STRING_LENGTH      ( 512 );

    m_ui->lineEdit_fis_no->setMaxLength                     ( 10   );
    m_ui->lineEdit_belge_numarasi->setMaxLength             ( 10   );
    m_ui->lineEdit_cari_hesap_ismi->setMaxLength            ( 60  );
    SET_COMBOBOX_LENGTH                                     ( m_ui->comboBox_vergi_dairesi, 50 );

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {
        m_ui->pushButton_detaylar->setHidden(true);
    }

    M_STOK_DETAYLARI->kdv_muaf_mi = 0;
    M_STOK_DETAYLARI->otv_muaf_mi = 0;

    int     point_size  = m_ui->tableWidget_fis_satirlari->font().pointSize();
    QFont   font        = m_ui->tableWidget_fis_satirlari->font();

    font.setPointSize ( point_size -2 );
    m_ui->tableWidget_fis_satirlari->horizontalHeader()->setFont ( font );

    FIS_EKRANINI_DUZENLE ();

    FOCUS_FIRST_WIDGET                                  ();
    AUTOSTART_FIS_KERNEL                                ( m_fis_id );
}

/**************************************************************************************
                   STOK_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void STOK_FISI::CLEAR_FORM_MEMBERS ()
{
    M_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI.clear();
    M_KDV_ORANLARINA_GORE_KDV_TUTARLARI.clear();
    M_OTV_ORANLARINA_GORE_OTV_TUTARLARI.clear();

    m_last_urun_id               =  -1;
    M_FIS->yazdirildi_mi         =   0;
    M_FIS->cari_hesap_id         =   0;
    M_FIS->iade_edilen_irs_id    =   0;
    M_FIS->iptal_mi              =   0;
    M_FIS->modul_id              =  -1;
    M_FIS->yazdirildi_mi         =   0;

    M_FIS->brut_tutar = 0.00;
    M_FIS->isk_oncesi_otv_tutari = 0.00;
    M_FIS->isk_oncesi_otv_dahil_tutar= 0.00;
    M_FIS->isk_oncesi_kdv_tutari = 0.00;
    M_FIS->isk_oncesi_kdv_dahil_tutar= 0.00;
    M_FIS->ara_toplam = 0.00;
    M_FIS->isk_sonrasi_otv_tutari = 0.00;
    M_FIS->isk_sonrasi_otv_dahil_tutar  = 0.00;
    M_FIS->isk_sonrasi_kdv_tutari = 0.00;
    M_FIS->irsaliye_tutari = 0.00;
    M_FIS->iskonto_yuzdesi = 0.00;
    M_FIS->iskonto_tutari  = 0.00;
    M_FIS->doviz_tutar_bilgileri = 0.00;
    M_FIS->kdv_oran_bilgileri = 0.00;
    M_FIS->otv_oran_bilgileri = 0.00;
    M_FIS->doviz_kuru = 0.00;
    M_FIS->parite = 0.00;

}

/**************************************************************************************
                   STOK_FISI::NEW_FIS_RECORD
***************************************************************************************/

void STOK_FISI::NEW_FIS_RECORD()
{

    m_ui->label_temel_para_birimi->setHidden        ( true  );
    m_ui->commaEdit_kur->setHidden                  ( true  );
    m_ui->label_cari_para_birimi->setHidden         ( true  );
    m_ui->label_stk_irs_para_birimi_1->setHidden    ( true  );
    m_ui->label_stk_irs_para_birimi_2->setHidden    ( true  );
    m_ui->commaEdit_parite->setHidden               ( true  );
    m_ui->label_parite->setHidden                   ( true  );

    m_ui->lineEdit_fis_no->setText("*");

    M_STOK_DETAYLARI->kdv_muaf_mi = 0;
    M_STOK_DETAYLARI->otv_muaf_mi = 0;

    FIS_EKRANINI_DUZENLE ();

    m_ui->textEdit_fatura_bilgisi->setHidden    ( true );

    int depo_id = SUBE_GET_UNITENIN_DEPOSU();

    QSearchEdit    *       islem_deposu;
    QLineEdit      *       islem_depo_adi;


    if ( m_depo_islem_yetkisi EQ SADECE_UNITE_DEPOSUNDA ) {

        m_ui->searchEdit_depo_kodu->SET_TEXT        ("");
        m_ui->lineEdit_depo_adi->clear              ();

        m_ui->searchEdit_alan_depo_kodu->SET_TEXT   ("");
        m_ui->lineEdit_alan_depo_adi->clear         ();

        if ( M_FIS->fis_turu EQ ENUM_STK_DEPO_TRANSFER_GIRIS ) {
            M_FIS->alan_depo_id      =   depo_id;
            islem_deposu        =   m_ui->searchEdit_alan_depo_kodu;
            islem_depo_adi      =   m_ui->lineEdit_alan_depo_adi;

            m_ui->label_depo->setHidden(false);
            m_ui->searchEdit_depo_kodu->setHidden(false);
            m_ui->lineEdit_depo_adi->setHidden(false);
            m_ui->label_alan_depo->setHidden(true);
            m_ui->searchEdit_alan_depo_kodu->setHidden(true);
            m_ui->lineEdit_alan_depo_adi->setHidden(true);
        }
        else {
            M_FIS->depo_id           =   depo_id;
            islem_deposu        =   m_ui->searchEdit_depo_kodu;
            islem_depo_adi      =   m_ui->lineEdit_depo_adi;

            m_ui->label_depo->setHidden(true);
            m_ui->searchEdit_depo_kodu->setHidden(true);
            m_ui->lineEdit_depo_adi->setHidden(true);
        }

    }
    else {
        m_ui->searchEdit_depo_kodu->SET_TEXT        ("");
        m_ui->lineEdit_depo_adi->clear              ();
        M_FIS->alan_depo_id = -1;
        M_FIS->depo_id      = -1;
        m_ui->searchEdit_alan_depo_kodu->SET_TEXT   ("");
        m_ui->lineEdit_alan_depo_adi->clear         ();

        if ( M_FIS->fis_turu EQ ENUM_STK_DEPO_TRANSFER_GIRIS ) {
            M_FIS->alan_depo_id      =   depo_id;
            islem_deposu        =   m_ui->searchEdit_alan_depo_kodu;
            islem_depo_adi      =   m_ui->lineEdit_alan_depo_adi;
        }
        else {
            M_FIS->depo_id           =   depo_id;
            islem_deposu        =   m_ui->searchEdit_depo_kodu;
            islem_depo_adi      =   m_ui->lineEdit_depo_adi;
        }

    }

    QString depo_adi, depo_kodu;
    SUBE_GET_DEPO_KODU_ADI(depo_id, &depo_kodu, &depo_adi );

    islem_deposu->SET_TEXT      ( depo_kodu );
    islem_depo_adi->setText     ( depo_adi );

    switch ( M_FIS->fis_turu ) {
        case ENUM_STK_STOK_ACILIS_FISI :
            m_ui->adakDate_fis_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
            m_ui->adakDate_fis_tarihi->setDisabled( true );
            break;

        default :
            break;
    }
}

/**************************************************************************************
                   STOK_FISI::GET_FIS_RECORD
***************************************************************************************/

int STOK_FISI::GET_FIS_RECORD ( int p_fis_id )
{
    SQL_QUERY   f_query   ( DB );
    SQL_QUERY   s_query   ( DB );

    STK_IRS_FIS_BILGILERINI_OKU( p_fis_id, M_FIS );

    FIS_EKRANINI_DUZENLE    ();

    m_ui->lineEdit_fis_no->setText( QString::number( M_FIS->fis_no ) );
    m_ui->adakDate_fis_tarihi->SET_DATE( M_FIS->fis_tarihi );
    m_ui->limitedTextEdit_aciklama->setText( M_FIS->aciklama );
    m_ui->textEdit_doviz_tutar_bilgileri->setText( M_FIS->doviz_tutar_bilgileri );

    QString depo_adi, depo_kodu;
    SUBE_GET_DEPO_KODU_ADI( M_FIS->depo_id, &depo_kodu, &depo_adi );

    m_ui->searchEdit_depo_kodu->SET_TEXT( depo_kodu );
    m_ui->lineEdit_depo_adi->setText( depo_adi  );

    m_ui->commaEdit_brut_tutar->SET_DOUBLE( M_FIS->brut_tutar );
    m_ui->commaEdit_ara_toplam->SET_DOUBLE( M_FIS->ara_toplam );
    m_ui->commaEdit_isk_sonrasi_otv_tutari->SET_DOUBLE( M_FIS->isk_sonrasi_otv_tutari );
    m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->SET_DOUBLE( M_FIS->isk_sonrasi_otv_dahil_tutar);
    m_ui->commaEdit_isk_sonrasi_kdv_tutari->SET_DOUBLE( M_FIS->isk_sonrasi_kdv_tutari );
    m_ui->commaEdit_irsaliye_tutari->SET_DOUBLE( M_FIS->irsaliye_tutari );
    m_ui->commaEdit_iskonto_yuzdesi->SET_DOUBLE( M_FIS->iskonto_yuzdesi );
    m_ui->commaEdit_iskonto_tutari->SET_DOUBLE( M_FIS->iskonto_tutari );

    m_ui->commaEdit_parite->SET_DOUBLE( M_FIS->parite );
    m_ui->commaEdit_kur->SET_DOUBLE( M_FIS->doviz_kuru );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_stk_irs_para_birimi_id, DVZ_GET_DOVIZ_KODU( M_FIS->stk_irs_para_birimi_id ) );

    //! doviz kuru ve parite

    IRS_KUR_VE_PARITE_AYARLA( false );

    if ( M_FIS->iade_edilen_irs_id > 0 ) {
        s_query.PREPARE_SELECT ( "stk_fisler","fis_no","fis_id = :fis_id");
        s_query.SET_VALUE      ( ":fis_id", M_FIS->iade_edilen_irs_id );

        if ( s_query.SELECT() > 0 ) {
            s_query.NEXT();
            m_ui->commaEdit_iade_edilecek_fis_no->SET_INTEGER ( s_query.VALUE(0).toInt() );
        }
    }

    //!
    double cari_carpani = M_FIS->doviz_kuru;
    if( M_FIS->stk_irs_para_birimi_id NE m_temel_para_birimi_id AND M_FIS->cari_para_birim_id NE m_temel_para_birimi_id ) {
        if( M_FIS->stk_irs_para_birimi_id NE M_FIS->cari_para_birim_id ) {
            cari_carpani = M_FIS->parite;
        }
        else {
            cari_carpani = 1.0000;
        }
    }

    m_ui->commaEdit_cari_para_birimi_tutari->SET_DOUBLE( ROUND( M_FIS->irsaliye_tutari * cari_carpani ) );

    M_STOK_DETAYLARI->kdv_muaf_mi  = M_FIS->kdv_muaf_mi;
    M_STOK_DETAYLARI->otv_muaf_mi  = M_FIS->otv_muaf_mi;
    M_STOK_DETAYLARI->muh_hesap_id = M_FIS->muh_hesap_id;

    m_ui->lineEdit_irsaliye_belge_seri->setText( M_FIS->belge_seri );
    m_ui->lineEdit_belge_numarasi->setText( M_FIS->belge_numarasi );

    m_ui->limitedTextEdit_adres->setText( M_FIS->teslim_adresi );
    m_ui->adakDate_sevk_tarihi->SET_DATE( M_FIS->sevk_tarihi  );
    m_ui->textEdit_kdv_yuzdeleri->setText( M_FIS->kdv_oran_bilgileri );
    m_ui->textEdit_otv_yuzdeleri->setText( M_FIS->otv_oran_bilgileri );

    if (  M_FIS->iptal_mi EQ 1 ) {
        m_ui->label_iptal->setVisible ( true );
        m_ui->button_yazdir->setVisible( false );
    }


    if ( M_FIS->cari_hesap_id > 0 ) {

        QString cari_hesap_kodu, cari_hesap_ismi;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( M_FIS->cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );
        if ( result > 0 ) {
            m_ui->searchEdit_cari_hesap_kodu->SET_TEXT ( cari_hesap_kodu );

            if ( M_FIS->yazdirildi_mi EQ 0 ) {
                cari_hesap_ismi    = cari_hesap_ismi;
            }
            m_ui->lineEdit_cari_hesap_ismi->setReadOnly ( true );
        }
        CARI_BORC_BILGISINI_GOSTER( M_FIS->cari_hesap_id );
    }

    m_ui->lineEdit_cari_hesap_ismi->setText    ( M_FIS->cari_hesap_ismi );
    m_ui->lineEdit_vergi_no->setText           ( M_FIS->vergi_numarasi  );
    FIND_AND_SET_COMBOBOX_TEXT(m_ui->comboBox_vergi_dairesi , M_FIS->vergi_dairesi );

    if ( M_FIS->alan_depo_id > 0 ) {

        QString depo_adi, depo_kodu;
        SUBE_GET_DEPO_KODU_ADI( M_FIS->alan_depo_id, &depo_kodu, &depo_adi );

        m_ui->searchEdit_alan_depo_kodu->SET_TEXT ( depo_kodu );
        m_ui->lineEdit_alan_depo_adi->setText     ( depo_adi  );

    }

    //kapali eger varsa acilacak
    m_ui->textEdit_fatura_bilgisi->setVisible( false );
    switch ( M_FIS->fis_turu ) {

        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS  :
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS : {

            m_ui->textEdit_fatura_bilgisi->setHidden(true);

            QList <int> fat_fis_id_list = FAT_GET_FAT_IRS_BAGLANTISI_FATURA_ID( p_fis_id );

            if ( fat_fis_id_list.size() EQ 0 ) {
                break;
            }
            FATURA_FISI_STRUCT FAT_FIS;

            FAT_FIS.KDV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
            FAT_FIS.KDV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
            FAT_FIS.KDV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

            FAT_FIS.OTV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
            FAT_FIS.OTV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
            FAT_FIS.OTV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

            FAT_FIS.TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY =   new int     [KDV_OTV_ORANLARI_SAYISI];

            for ( int i = 0; i < fat_fis_id_list.size(); i++ ) {

                FATURA_CLEAR_FIS_STRUCT ( &FAT_FIS );

                FATURA_FIS_BILGILERINI_OKU( &FAT_FIS , fat_fis_id_list.at( i ) );

                m_ui->tableWidget_irsaliyenin_faturalari->insertRow(m_ui->tableWidget_irsaliyenin_faturalari->rowCount());

                int last_row = m_ui->tableWidget_irsaliyenin_faturalari->rowCount() - 1;

                QTableWidgetItem * fat_id_item         =   new QTableWidgetItem ( fat_fis_id_list.at(i));
                QTableWidgetItem * irs_id_item         =   new QTableWidgetItem ( QVariant ( p_fis_id ).toString() );
                QTableWidgetItem * fat_seri_no_item    =   new QTableWidgetItem ( FAT_FIS.belge_seri + " " + FAT_FIS.belge_numarasi );
                QTableWidgetItem * fat_tarihi_item     =   new QTableWidgetItem ( QDate::fromString ( FAT_FIS.fatura_tarihi, "yyyy.MM.dd" ).toString("dd MM yyyy"));

                m_ui->tableWidget_irsaliyenin_faturalari->setItem ( last_row, 0, fat_id_item      );
                m_ui->tableWidget_irsaliyenin_faturalari->setItem ( last_row, 1, irs_id_item      );
                m_ui->tableWidget_irsaliyenin_faturalari->setItem ( last_row, 2, fat_seri_no_item );
                m_ui->tableWidget_irsaliyenin_faturalari->setItem ( last_row, 3, fat_tarihi_item  );

            }
            break;
        }
        case  ENUM_STK_SARF_FIRE_FISI        :
        case  ENUM_STK_IMALATA_SEVK_FISI     :
        case  ENUM_STK_IMALATTAN_DONUS_FISI  :
        case  ENUM_STK_STOK_ACILIS_FISI      :
        case  ENUM_STK_DEPO_TRANSFER_CIKIS   :
        case  ENUM_STK_DEPO_TRANSFER_GIRIS   :
        case  ENUM_IRS_FASON_BASLANGIC_CIKIS :
        case  ENUM_IRS_FASON_BASLANGIC_GIRIS :
        case  ENUM_IRS_FASON_BITIS_CIKIS     :
        case  ENUM_IRS_FASON_BITIS_GIRIS     :
            m_ui->textEdit_fatura_bilgisi->setHidden(true);
            break;
        default :
            break;
    }

    IRS_BAGLANTI_BILGISINI_GOSTER( M_FIS->fis_turu, p_fis_id );


    f_query.PREPARE_SELECT ( "stk_fis_satirlari",
                             "fis_satiri_id, order_number",
                             "fis_id = :fis_id " );

    f_query.SET_VALUE(":fis_id", p_fis_id );

    if ( f_query.SELECT( "order_number" ) EQ 0 ) {
        return ADAK_OK;
    }

    int row_number = 0;
    QTableWidgetItem * row_item;

    while ( f_query.NEXT() EQ true ) {

        row_number = ADD_NEW_LINE ();

        int    irsaliye_satiri_id   = f_query.VALUE("fis_satiri_id").toInt();

        row_item   = new QTableWidgetItem ( QVariant ( irsaliye_satiri_id ).toString() );
        m_ui->tableWidget_fis_satirlari->setItem ( row_number, ROW_ID_COLUMN, row_item );
        
        row_item   = new QTableWidgetItem ( QVariant ( p_fis_id ).toString() );
        m_ui->tableWidget_fis_satirlari->setItem ( row_number, FIS_ID_COLUMN, row_item );
        
        row_item   = new QTableWidgetItem( QString ( f_query.VALUE("order_number").toString() ) );
        m_ui->tableWidget_fis_satirlari->setItem ( row_number, ORDER_NUM_COLUMN, row_item );

        STK_IRS_CLEAR_SATIR_STRUCT( M_FIS_SATIRI );
        STK_IRS_SATIR_BILGILERINI_OKU( irsaliye_satiri_id, M_FIS_SATIRI );

        QLineEdit    * lineEdit_seri_no_list                     = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, SERI_NO_LISTE_COLUMN                   );
        QLineEdit    * lineEdit_urun_id                          = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, URUN_ID_COLUMN                        );
        QSearchEdit  * searchEdit_urun_kodu                      = ( QSearchEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, URUN_KOD_COLUMN                      );
        QLineEdit    * lineEdit_urun_adi                         = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, URUN_AD_COLUMN                       );
        QCommaEdit   * commaEdit_satir_stok_miktari              = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, STOK_MIKTARI_COLUMN                   );
        QComboBox    * comboBox_urun_birimi                      = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, URUN_BIRIMI_COLUMN                    );
        QCommaEdit   * commaEdit_temel_birim_katsayisi           = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, TEMEL_BIRIM_KATSAYISI                 );
        QCommaEdit   * commaEdit_isk_oncesi_birim_fiyat          = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN         );
        QCommaEdit   * commaEdit_isk_sonrasi_birim_fiyat         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN        );
        QCommaEdit   * commaEdit_iskonto_yuzdesi                 = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, ISKONTO_YUZDESI_COLUMN                );
        QCommaEdit   * commaEdit_iskonto_tutari                  = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, ISKONTO_TUTARI_COLUMN                 );
        QCommaEdit   * commaEdit_satir_tutari                    = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, SATIR_TUTARI_COLUMN                   );
        QLineEdit    * lineEdit_para_birimi                      = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, PARA_BIRIMI_COLUMN                    );
        QCommaEdit   * commaEdit_vergi_haric_tutar               = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, VERGI_HARIC_TUTARI_COLUMN              );
        QLineEdit    * lineEdit_stok_takip_sekli                 = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, STOK_TAKIP_SEKLI_COLUMN               );
        QLineEdit    * lineEdit_parti_id                         = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, PARTI_ID_COLUMN                       );
        QPushButton  * pushButton_seri_no_parti_kodu             = ( QPushButton * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, SERI_NO_PARTI_KODU_COLUMN             );
        QComboBox    * comboBox_kdv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, KDV_ORANI_COLUMN                      );
        QCommaEdit   * commaEdit_kdv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, KDV_TUTAR_COLUMN                     );
        QCommaEdit   * commaEdit_kdv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, KDV_DAHIL_TUTARI_COLUMN                );
        QComboBox    * comboBox_kdv_dahil_mi                     = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, KDV_DAHIL_MI_COLUMN                   );
        QCommaEdit   * commaEdit_otv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, OTV_TUTARI_COLUMN                     );
        QCommaEdit   * commaEdit_otv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, OTV_DAHIL_TUTAR_COLUMN                );
        QComboBox    * comboBox_otv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, OTV_ORANI_COLUMN                      );
        QLineEdit    * lineEdit_fatura_satiri_id                 = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, FATURA_SATIRI_ID_COLUMN               );
        QLineEdit    * lineEdit_hatali_satir_mi                  = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, HATALI_SATIR_MI_COLUMN                );
        QLineEdit    * lineEdit_iade_edilen_irs_satir_id         = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, IADE_EDILEN_IRS_SATIR_ID_COLUMN       );
        QCommaEdit   * commaEdit_toplu_iskonto_yuzdesi           = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, TOPLU_ISKONTO_YUZDESI_COLUMN          );
        QCommaEdit   * commaEdit_t_is_sonrasi_birim_fiyat        = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, T_IS_SONRASI_BIRIM_FIYAT_COLUMN       );
        QCommaEdit   * commaEdit_t_is_sonrasi_satir_tutari       = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, T_IS_SONRASI_SATIR_TUTARI_COLUMN      );
        QCommaEdit   * commaEdit_t_is_sonrasi_vergi_haric_tutar  = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN );
        QCommaEdit   * commaEdit_t_is_sonrasi_otv_tutari         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, T_IS_SONRASI_OTV_TUTARI_COLUMN        );
        QCommaEdit   * commaEdit_t_is_sonrasi_otv_dahil_tutar    = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN   );
        QCommaEdit   * commaEdit_t_is_sonrasi_kdv_tutari         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, T_IS_SONRASI_KDV_TUTARI_COLUMN        );
        QCommaEdit   * commaEdit_t_is_sonrasi_kdv_dahil_tutar    = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN   );


        STK_FILL_URUN_BIRIMLERI_COMBOBOX ( M_FIS_SATIRI->urun_id, comboBox_urun_birimi, M_STOK_BIRIM_ADLARI );

        int valid_comboBox_index = comboBox_urun_birimi->findText ( M_FIS_SATIRI->urun_birimi );

        if ( valid_comboBox_index > -1 ) {
            comboBox_urun_birimi->setCurrentIndex(valid_comboBox_index);
        }
        else {
            comboBox_urun_birimi->setEditText( M_FIS_SATIRI->urun_birimi );
        }

        //! FIXME SATIRDA DOVIZ ID OLAMAZ.

        lineEdit_para_birimi->setText( DVZ_GET_DOVIZ_KODU( M_FIS_SATIRI->doviz_id ) );

        commaEdit_vergi_haric_tutar->SET_DOUBLE                 ( M_FIS_SATIRI->satir_vergi_haric_tutar );
        commaEdit_isk_oncesi_birim_fiyat->SET_DOUBLE            ( M_FIS_SATIRI->isk_oncesi_urun_birim_fiyati  );
        commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE           ( M_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati );
        commaEdit_iskonto_yuzdesi->SET_DOUBLE                   ( M_FIS_SATIRI->iskonto_yuzdesi );
        commaEdit_iskonto_tutari->SET_DOUBLE                    ( M_FIS_SATIRI->iskonto_tutari );
        commaEdit_temel_birim_katsayisi->SET_DOUBLE             ( M_FIS_SATIRI->temel_birim_katsayisi);
        commaEdit_satir_tutari->SET_DOUBLE                      ( M_FIS_SATIRI->satir_tutari                                                    );
        commaEdit_toplu_iskonto_yuzdesi->SET_DOUBLE             ( M_FIS_SATIRI->toplu_iskonto_yuzdesi );
        commaEdit_t_is_sonrasi_birim_fiyat->SET_DOUBLE          ( M_FIS_SATIRI->t_is_sonrasi_birim_fiyat );
        commaEdit_t_is_sonrasi_satir_tutari->SET_DOUBLE         ( M_FIS_SATIRI->t_is_sonrasi_satir_tutari );
        commaEdit_t_is_sonrasi_vergi_haric_tutar->SET_DOUBLE    ( M_FIS_SATIRI->t_is_sonrasi_vergi_haric_tutar );
        commaEdit_t_is_sonrasi_otv_tutari->SET_DOUBLE           ( M_FIS_SATIRI->t_is_sonrasi_otv_tutari );
        commaEdit_t_is_sonrasi_otv_dahil_tutar->SET_DOUBLE      ( M_FIS_SATIRI->t_is_sonrasi_otv_dahil_tutar );
        commaEdit_t_is_sonrasi_kdv_tutari->SET_DOUBLE           ( M_FIS_SATIRI->t_is_sonrasi_kdv_tutari );
        commaEdit_t_is_sonrasi_kdv_dahil_tutar->SET_DOUBLE      ( M_FIS_SATIRI->t_is_sonrasi_kdv_dahil_tutar );

        lineEdit_urun_id->setText( QVariant ( M_FIS_SATIRI->urun_id ).toString() );
        lineEdit_urun_adi->setText( M_FIS_SATIRI->urun_adi );
        lineEdit_stok_takip_sekli->setText  ( "-1" );

        int stok_takip_sekli = -1;


        if ( M_FIS_SATIRI->urun_id > 0 ) {
            s_query.PREPARE_SELECT ("stk_urunler","urun_kodu, urun_adi, stok_takip_sekli",
                                    "urun_id = :urun_id");
            s_query.SET_VALUE(":urun_id", M_FIS_SATIRI->urun_id );
            if ( s_query.SELECT() > 0 ) {
                s_query.NEXT();
                searchEdit_urun_kodu->SET_TEXT      ( s_query.VALUE(0).toString() );
                lineEdit_urun_adi->setText          ( s_query.VALUE(1).toString() );
                lineEdit_stok_takip_sekli->setText  ( s_query.VALUE(2).toString() );
                lineEdit_urun_adi->setReadOnly      ( true );
                stok_takip_sekli = s_query.VALUE(2).toInt();

                if ( stok_takip_sekli EQ ENUM_BASIT_TAKIP ) {
                    pushButton_seri_no_parti_kodu->setEnabled(false);
                }
            }
        }

        commaEdit_satir_stok_miktari->SET_DOUBLE( M_FIS_SATIRI->satirdaki_urun_miktari );


        switch ( stok_takip_sekli ) {

            case ENUM_PARTI_TAKIBI :
                if ( M_FIS_SATIRI->parti_id > 0 ) {
                    lineEdit_seri_no_list->setText(STK_GET_PARTI_KODU( M_FIS_SATIRI->parti_id ));
                    lineEdit_parti_id->setText(QVariant( M_FIS_SATIRI->parti_id ).toString());
                }
                break;
            case ENUM_SERI_NO_ILE_TAKIP : {

                commaEdit_satir_stok_miktari->setDisabled( true );
                comboBox_urun_birimi->setDisabled( true );

                // SERI NOLAR her satirin ayri seri nosu vardir search edite tiklandiktan sonra
                // burari parse edilir. ve batch gonderilir.

                QString seri_numaralari = STK_GET_SERI_NO_BILGILERI( irsaliye_satiri_id , ENUM_URUN_HER_IKISI, -1  );

                lineEdit_seri_no_list->setText( seri_numaralari );

                if ( M_FIS->fis_turu EQ ENUM_STK_STOK_ACILIS_FISI ) {
                    break;
                }
                break;
            }
            case ENUM_BASIT_TAKIP  :
            default                :
                break;
        }

        switch ( M_FIS->fis_turu ) {

            case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS      :
            case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS     :
            case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS       :
            case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS      :

                s_query.PREPARE_SELECT ( "fat_irs_satir_baglantisi","fatura_satiri_id",
                                         "irsaliye_satiri_id = :irsaliye_satiri_id");
                s_query.SET_VALUE(":irsaliye_satiri_id", irsaliye_satiri_id );
                if ( s_query.SELECT() EQ 0 ) {
                    break;
                }
                s_query.NEXT();
                lineEdit_fatura_satiri_id->setText ( s_query.VALUE(0).toString());


                commaEdit_satir_tutari->setEnabled              ( false );
                commaEdit_isk_oncesi_birim_fiyat->setEnabled    ( false );
                commaEdit_isk_sonrasi_birim_fiyat->setEnabled   ( false );
                commaEdit_iskonto_yuzdesi->setEnabled           ( false );
                lineEdit_urun_adi->setEnabled                   ( false );
                searchEdit_urun_kodu->setEnabled                ( false );
                comboBox_urun_birimi->setEnabled                ( false );
                lineEdit_para_birimi->setEnabled                ( false );
                comboBox_kdv_orani->setEnabled                  ( false );
                comboBox_otv_orani->setEnabled                  ( false );
                comboBox_kdv_dahil_mi->setEnabled               ( false );

                if ( M_FIS->fis_turu EQ ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS OR
                     M_FIS->fis_turu EQ ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS ) {
                    commaEdit_satir_stok_miktari->setEnabled( false );
                }

                break;
            default :
                break;
        }

        if ( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
            s_query.PREPARE_SELECT ( "irs_iade_satir_baglantisi",
                                     "irs_iade_edilen_satir_id ",
                                     "irs_satir_id = :irs_satir_id");
            s_query.SET_VALUE      ( ":irs_satir_id", irsaliye_satiri_id );
            if ( s_query.SELECT() > 0 ) {
                s_query.NEXT();
                lineEdit_iade_edilen_irs_satir_id->setText ( s_query.VALUE(0).toString() );

                commaEdit_satir_tutari->setEnabled              ( false );
                commaEdit_isk_oncesi_birim_fiyat->setEnabled    ( false );
                commaEdit_isk_sonrasi_birim_fiyat->setEnabled   ( false );
                commaEdit_iskonto_yuzdesi->setEnabled           ( false );
                lineEdit_urun_adi->setEnabled                   ( false );
                searchEdit_urun_kodu->setEnabled                ( false );
                comboBox_urun_birimi->setEnabled                ( false );
                lineEdit_para_birimi->setEnabled                ( false );
                comboBox_kdv_orani->setEnabled                  ( false );
                comboBox_otv_orani->setEnabled                  ( false );
                comboBox_kdv_dahil_mi->setEnabled               ( false );
            }
        }

        comboBox_kdv_orani->setCurrentIndex     ( comboBox_kdv_orani->findText ( QString::number( M_FIS_SATIRI->kdv_orani)) );
        commaEdit_kdv_tutari->SET_DOUBLE        ( M_FIS_SATIRI->satir_kdv_tutari );
        commaEdit_kdv_dahil_tutar->SET_DOUBLE   ( M_FIS_SATIRI->satir_kdv_dahil_tutar );
        comboBox_kdv_dahil_mi->setCurrentIndex  ( M_FIS_SATIRI->kdv_dahil_mi );

        comboBox_otv_orani->setCurrentIndex     ( comboBox_otv_orani->findText ( QString::number( M_FIS_SATIRI->otv_orani ) ) );
        commaEdit_otv_tutari->SET_DOUBLE        ( M_FIS_SATIRI->satir_otv_tutari );
        commaEdit_otv_dahil_tutar->SET_DOUBLE   ( M_FIS_SATIRI->satir_otv_dahil_tutar );
        lineEdit_hatali_satir_mi->setText       ( QString::number( M_FIS_SATIRI->hatali_satir_mi ) );

        SATIR_DOVIZ_TUTARLARINI_GUNCELLE  ( lineEdit_para_birimi->text(), commaEdit_satir_tutari->GET_DOUBLE(), DOVIZ_EKLE );
        KDV_ORANLARI_BILGILERINI_GUNCELLE ( comboBox_kdv_orani->currentText(), commaEdit_kdv_tutari->GET_DOUBLE(), VERGI_ORANI_EKLE );
        OTV_ORANLARI_BILGILERINI_GUNCELLE ( comboBox_otv_orani->currentText(), commaEdit_otv_tutari->GET_DOUBLE(), VERGI_ORANI_EKLE );

    }
    // FIS BILGILERI YAZILIYOR.

    SATIR_DOVIZ_TUTARLARINI_YAZDIR    ();
    KDV_ORANLARI_BILGILERINI_YAZDIR   ();
    OTV_ORANLARI_BILGILERINI_YAZDIR   ();

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void STOK_FISI::SET_LINE_DEFAULTS ( int p_row_number )
{
    QCommaEdit  * commaEdit_satirdaki_urun_miktari      = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_MIKTARI_COLUMN                   );
    QCommaEdit  * commaEdit_temel_birim_katsayisi       = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, TEMEL_BIRIM_KATSAYISI                 );
    QLineEdit   * lineEdit_para_birimi                  = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, PARA_BIRIMI_COLUMN                    );
    QCommaEdit  * commaEdit_vergi_haric_tutar           = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTARI_COLUMN              );
    QLineEdit   * lineEdit_urun_adi                     = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_AD_COLUMN                       );
    QComboBox   * comboBox_kdv_dahil_mi                 = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN                   );
    QComboBox   * comboBox_kdv_orani                    = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_ORANI_COLUMN                      );
    QComboBox   * comboBox_otv_orani                    = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_ORANI_COLUMN                      );
    QPushButton * button_fat_satiri_secimi              = ( QPushButton * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, FATURA_SATIRI_SECIMI_COLUMN           );
    QPushButton * button_iade_irsaliye_satiri_secimi    = ( QPushButton * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, IADE_IRSALIYE_SATIRI_SECIMI_COLUMN    );
    QPushButton * pushButton_seri_no_parti_kodu         = ( QPushButton * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SERI_NO_PARTI_KODU_COLUMN            );
    QCommaEdit  * commaEdit_iskonto_oncesi_birim_fiyat  = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN         );
    QCommaEdit  * commaEdit_iskonto_sonrasi_birim_fiyat = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN        );

    commaEdit_temel_birim_katsayisi->SET_DOUBLE(1);

    lineEdit_urun_adi->setMaxLength ( 50 );

    E9_FILL_KDV_ORANLARI_COMBOBOX      ( comboBox_kdv_orani   );
    E9_FILL_OTV_ORANLARI_COMBOBOX      ( comboBox_otv_orani   );

    comboBox_kdv_orani->setCurrentIndex(comboBox_kdv_orani->findText(QVariant(m_ontanimli_kdv_orani).toString()));

    commaEdit_vergi_haric_tutar->setEnabled ( false );

    lineEdit_para_birimi->setMaxLength( 5 );

    lineEdit_para_birimi->setText( m_temel_para_birimi_kodu );

    comboBox_kdv_dahil_mi->addItems    ( QStringList() << tr("Hariç") << tr("Dahil") );
    comboBox_kdv_dahil_mi->setEditable ( false );

    button_fat_satiri_secimi->setToolTip(tr("Fatura satırı seçmek için tıklayın"));

    button_fat_satiri_secimi->setIcon( QPixmap( ":/e9_icons/E9_ICONS/fatura.png" ));
    button_iade_irsaliye_satiri_secimi->setIcon( QPixmap( ":/e9_icons/E9_ICONS/irsaliye.png" ));

    pushButton_seri_no_parti_kodu->setIconSize( QSize( 80, 20 ));
    pushButton_seri_no_parti_kodu->setIcon( QPixmap( ":/e9_icons/E9_ICONS/stok_seri_numaralari.png" ));

    button_iade_irsaliye_satiri_secimi->setToolTip(tr("İade irsaliye satırı seçmek için tıklayın"));

    pushButton_seri_no_parti_kodu->setToolTip( tr( "Seri numarası veya Parti kodu girmek için tıklayın." ) );

    REGISTER_LINE_CHANGER_BUTTON ( p_row_number, FATURA_SATIRI_SECIMI_COLUMN        );
    REGISTER_LINE_CHANGER_BUTTON ( p_row_number, IADE_IRSALIYE_SATIRI_SECIMI_COLUMN );
    REGISTER_LINE_CHANGER_BUTTON ( p_row_number, SERI_NO_PARTI_KODU_COLUMN         );

    if ( M_STOK_DETAYLARI->kdv_muaf_mi EQ 1 ) {
        comboBox_kdv_orani->setCurrentIndex(comboBox_kdv_orani->findText("0"));
        comboBox_kdv_orani->setDisabled(true);
    }
    else {
        comboBox_kdv_orani->setEnabled(true);
    }

    if ( M_STOK_DETAYLARI->otv_muaf_mi EQ 1 ) {
        comboBox_otv_orani->setCurrentIndex(comboBox_otv_orani->findText("0"));
        comboBox_otv_orani->setDisabled(true);
    }
    else {
        comboBox_otv_orani->setEnabled(true);
    }

    // [ 1 ] Hassasiyet setleniyor
    int stok_birim_fiyat_hassasiyeti = STK_GET_BIRIM_HASSASIYETI();

    commaEdit_iskonto_oncesi_birim_fiyat->SET_PRECISION( stok_birim_fiyat_hassasiyeti );
    commaEdit_iskonto_sonrasi_birim_fiyat->SET_PRECISION( stok_birim_fiyat_hassasiyeti );

    // [ 1 ] Hassasiyet setleniyor
    int stok_birim_miktar_hassasiyeti = STK_GET_MIKTAR_HASSASIYETI();

    commaEdit_satirdaki_urun_miktari->SET_PRECISION( stok_birim_miktar_hassasiyeti );

    lineEdit_para_birimi->setEnabled( false );

}

/**************************************************************************************
                   STOK_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int STOK_FISI::CHECK_FIS_FORM_VAR ( QObject * p_object )
{

    if ( p_object EQ m_ui->comboBox_vergi_dairesi ) {
        if ( m_ui->comboBox_vergi_dairesi->currentText().size() > 50 ) {
            MSG_WARNING(  tr("Vergi dairesi isminin uzunluğu 50 karakterden fazla olamaz."), m_ui->comboBox_vergi_dairesi );
            return ADAK_FAIL;
        }
    }
    else if ( p_object EQ m_ui->searchEdit_depo_kodu OR p_object EQ m_ui->searchEdit_alan_depo_kodu ) {

        if ( p_object EQ m_ui->searchEdit_depo_kodu ) {
            if ( m_ui->searchEdit_depo_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineEdit_depo_adi->clear();
                M_FIS->depo_id = -1;
                return ADAK_OK;
            }
        }
        else {
            if ( m_ui->searchEdit_alan_depo_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineEdit_alan_depo_adi->clear();
                M_FIS->alan_depo_id = -1;
                return ADAK_OK;
            }
        }

        QString depo_adi;
        int     depo_id;

        int     result = 0;

        if ( p_object EQ m_ui->searchEdit_depo_kodu ) {
            result = SUBE_GET_DEPO_ID_ADI( m_ui->searchEdit_depo_kodu->GET_TEXT(), &depo_id, &depo_adi );
        }
        else {
            result = SUBE_GET_DEPO_ID_ADI( m_ui->searchEdit_alan_depo_kodu->GET_TEXT(), &depo_id, &depo_adi );
        }
        if  ( result EQ 0 ) {
            MSG_WARNING(  tr("Depo kodu bulunamadı"), m_ui->searchEdit_depo_kodu );

            return ADAK_FAIL_UNDO;
        }
        if ( p_object EQ m_ui->searchEdit_depo_kodu ) {
            M_FIS->depo_id = depo_id;
            if ( M_FIS->depo_id EQ M_FIS->alan_depo_id ) {
                MSG_WARNING(  tr("Alan depo kodu ile gönderen depo kodu aynı olamaz"), m_ui->searchEdit_alan_depo_kodu );
                return ADAK_FAIL_UNDO;
            }
            m_ui->lineEdit_depo_adi->setText ( depo_adi );
        }
        else {
            M_FIS->alan_depo_id = depo_id;
            if ( M_FIS->depo_id EQ M_FIS->alan_depo_id ) {
                MSG_WARNING(  tr("Alan depo kodu ile gönderen depo kodu aynı olamaz"), m_ui->searchEdit_alan_depo_kodu );

                return ADAK_FAIL_UNDO;
            }
            m_ui->lineEdit_alan_depo_adi->setText ( depo_adi );
        }
    }
    else if ( p_object EQ m_ui->searchEdit_cari_hesap_kodu ) {

        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            M_FIS->cari_hesap_id = -1;
            m_ui->lineEdit_cari_hesap_ismi->clear();
            m_ui->lineEdit_cari_hesap_ismi->setReadOnly ( false );
            m_ui->comboBox_vergi_dairesi->setEnabled(true);
            m_ui->lineEdit_vergi_no->setEnabled(true);
            return ADAK_OK;
        }

        QString cari_hesap_ismi;
        int     cari_hesap_id;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT(), &cari_hesap_id, &cari_hesap_ismi );


        if ( result EQ 0 ) {
            MSG_WARNING(  tr("Cari hesap bulunamadı"), m_ui->searchEdit_cari_hesap_kodu );

            return ADAK_FAIL_UNDO;
        }

        QString vergi_dairesi;
        QString vergi_no;

        CARI_GET_VERGI_NO_VERGI_DAIRESI( cari_hesap_id, &vergi_no, &vergi_dairesi );

        FIND_AND_SET_COMBOBOX_TEXT(m_ui->comboBox_vergi_dairesi , vergi_dairesi);
        m_ui->lineEdit_vergi_no->setText          ( vergi_no );
        m_ui->lineEdit_cari_hesap_ismi->setText   ( cari_hesap_ismi );

        if ( m_ui->lineEdit_vergi_no->text().isEmpty() EQ false AND
             m_ui->comboBox_vergi_dairesi->currentIndex() NE -1 ) {
            m_ui->comboBox_vergi_dairesi->setDisabled(true);
            m_ui->lineEdit_vergi_no->setDisabled(true);
        }
        else {
            m_ui->comboBox_vergi_dairesi->setEnabled(true);
            m_ui->lineEdit_vergi_no->setEnabled(true);
        }

        M_FIS->cari_hesap_id = cari_hesap_id;

        M_FIS->cari_para_birim_id = CARI_GET_PARA_BIRIM_ID( cari_hesap_id );
        M_FIS->cari_para_birim_kodu = DVZ_GET_DOVIZ_KODU( M_FIS->cari_para_birim_id );

        M_FIS->stk_irs_para_birimi_id = M_FIS->cari_para_birim_id;

        FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_stk_irs_para_birimi_id, M_FIS->cari_para_birim_kodu );

        if( M_FIS->cari_para_birim_id NE m_temel_para_birimi_id ) {

            m_ui->commaEdit_kur->SET_DOUBLE( DVZ_PARA_BIRIMLERINI_DONUSTUR ( M_FIS->cari_para_birim_id, 1.00, m_temel_para_birimi_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ) );

            m_ui->label_stk_irs_para_birimi_1->setVisible( true );
            m_ui->label_stk_irs_para_birimi_1->setText( "Kur  : 1 "+ DVZ_GET_DOVIZ_KODU( M_FIS->stk_irs_para_birimi_id ) );
            m_ui->commaEdit_kur->setVisible( true );
            m_ui->label_temel_para_birimi->setVisible( true );
            m_ui->label_parite->setVisible( false );
            m_ui->commaEdit_parite->setVisible( false );
            m_ui->label_cari_para_birimi->setVisible( false );
            m_ui->label_stk_irs_para_birimi_2->setVisible( false );
            m_ui->commaEdit_parite->clear();

        }

        QString adres_bilgileri = CARI_GET_ADRES_BILGILERI( cari_hesap_id );

        m_ui->limitedTextEdit_adres->setText ( adres_bilgileri );
        
        m_ui->lineEdit_cari_hesap_ismi->setReadOnly ( true  );

        CARI_BORC_BILGISINI_GOSTER( M_FIS->cari_hesap_id );

        SATIR_PARA_BIRIMINI_GUNCELLE();

    }
    else if ( p_object EQ m_ui->tableWidget_irsaliyenin_faturalari ) {
        if ( p_object->objectName() EQ "ITEM_DOUBLE_CLICKED" ) {
            int clicked_button = MSG_YES_NO( tr("Seçili faturaya ait satırlar irsaliyeden silinecektir. Emin misiniz?"), NULL );
            if ( clicked_button EQ ADAK_NO ) {
                return ADAK_OK;
            }
            int current_row = m_ui->tableWidget_irsaliyenin_faturalari->currentRow();
            SECILI_FATURANIN_SATIRLARINI_IRSALIYEDEN_SIL ( current_row );
        }
    }
    else if ( p_object EQ m_ui->comboBox_irsaliye_turu ) {
       M_FIS->fis_turu = STK_GET_STOK_FIS_TURU_ENUM ( m_ui->comboBox_irsaliye_turu->currentText() );

       if ( M_FIS->fis_turu EQ ENUM_STK_IMALATTAN_DONUS_FISI ) {
           M_STOK_DETAYLARI->muh_hesap_id = M_SUBE_ENT_HESAPLARI->imalattan_giris_hesap_id;
       }
       else {
           M_STOK_DETAYLARI->muh_hesap_id = M_SUBE_ENT_HESAPLARI->sarf_malzemeleri_hesap_id;
       }
       return ADAK_NEW_RECORD;
    }
    else if ( p_object EQ m_ui->lineEdit_belge_numarasi OR p_object EQ m_ui->lineEdit_irsaliye_belge_seri ) {
        m_ui->lineEdit_irsaliye_belge_seri->setText ( m_ui->lineEdit_irsaliye_belge_seri->text().toUpper() );
    }
    else if ( p_object EQ m_ui->commaEdit_iskonto_tutari OR p_object EQ m_ui->commaEdit_iskonto_yuzdesi ) {

        if ( p_object EQ m_ui->commaEdit_iskonto_tutari ) {

            if ( m_ui->commaEdit_iskonto_tutari->GET_DOUBLE() > M_FIS->brut_tutar ) {
                MSG_WARNING(  tr("Fatura vergi hariç toplamından daha büyük iskonto uygulayamazsınız."), m_ui->commaEdit_iskonto_tutari );

                return ADAK_FAIL_UNDO;
            }
            M_FIS->iskonto_tutari  = m_ui->commaEdit_iskonto_tutari->GET_DOUBLE();
            M_FIS->iskonto_yuzdesi = ROUND ( ( M_FIS->iskonto_tutari / M_FIS->brut_tutar ) * 100.00, -4 );
            m_ui->commaEdit_iskonto_yuzdesi->SET_DOUBLE ( M_FIS->iskonto_yuzdesi );
        }
        else {
            if ( m_ui->commaEdit_iskonto_yuzdesi->GET_DOUBLE() > 100.00 ) {
                MSG_WARNING(  tr("İskonto %100 den daha büyük olamaz."), m_ui->commaEdit_iskonto_yuzdesi );

                return ADAK_FAIL_UNDO;
            }
            M_FIS->iskonto_yuzdesi  = m_ui->commaEdit_iskonto_yuzdesi->GET_DOUBLE();
            M_FIS->iskonto_tutari   = ROUND ( ( M_FIS->iskonto_yuzdesi * M_FIS->brut_tutar ) / 100.00 );
            m_ui->commaEdit_iskonto_tutari->SET_DOUBLE ( M_FIS->iskonto_tutari );
        }

        ISKONTO_ORANINA_GORE_TOPLAMLARI_HESAPLA ();

        CLOSE_ALL_EVENTS    ();

        for ( int i = 0; i < m_ui->tableWidget_fis_satirlari->rowCount() -1; i++ ) {
            TOPLU_ISKONTO_SONRASI_SATIR_DEGERLERINI_HESAPLA ( i );
        }

        OPEN_ALL_EVENTS ();
    }
    else if ( p_object EQ m_ui->comboBox_stk_irs_para_birimi_id ) {
        M_FIS->stk_irs_para_birimi_id = DVZ_GET_DOVIZ_ID( m_ui->comboBox_stk_irs_para_birimi_id->currentText() ) ;

        //! KONTROL
        SQL_QUERY sql_query ( DB );
        int fis_id = GET_RECORD_ID();

        if ( fis_id NE -1 ) {
            sql_query.PREPARE_SELECT ( "stk_fis_satirlari","fis_satiri_id",
                                       "fis_id = :fis_id" );

            sql_query.SET_VALUE      ( ":fis_id", fis_id );

            if ( sql_query.SELECT() NE 0 ) {
                MSG_WARNING( tr ("Fiş satırlarında işlem bulunmaktadır.Irsaliye para birimini değiştiremezsiniz."), m_ui->comboBox_stk_irs_para_birimi_id );
                return ADAK_FAIL_UNDO;
            }
        }

        if( M_FIS->cari_hesap_id < 1 ) {
            MSG_WARNING( "Önce Cari Hesap Seçmelisiniz.!", m_ui->searchEdit_cari_hesap_kodu );
            return ADAK_FAIL;
        }
        //! END KONTROL
        M_FIS->stk_irs_para_birimi_id = DVZ_GET_DOVIZ_ID( m_ui->comboBox_stk_irs_para_birimi_id->currentText() );
        m_ui->commaEdit_kur->SET_DOUBLE( DVZ_DOVIZ_KURUNU_AL( M_FIS->stk_irs_para_birimi_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ) );

        IRS_KUR_VE_PARITE_AYARLA( true );

        SATIR_PARA_BIRIMINI_GUNCELLE();

    }

    return ADAK_OK;    
}

/**************************************************************************************
                   STOK_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int STOK_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( m_ui->lineEdit_fis_no->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr("Fiş numarasını boş bıramazsınız."), m_ui->lineEdit_fis_no );

        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_depo_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING(  tr("Depo kodu boş bırakılamaz"), m_ui->searchEdit_depo_kodu );

        return ADAK_FAIL;
    }
    if ( M_FIS->depo_id < 1 ) {
        MSG_WARNING(  tr("Depo seçimi yapılmamış"), m_ui->searchEdit_depo_kodu );

        return ADAK_FAIL;
    }

    switch ( M_FIS->fis_turu ) {
        case ENUM_IRS_NORMAL_GIRIS:
        case ENUM_IRS_FASON_BITIS_GIRIS :
        case ENUM_STK_DEPO_TRANSFER_GIRIS :
        case ENUM_IRS_FASON_BASLANGIC_GIRIS:
        case ENUM_IRS_KONSINYE_GIRIS :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS:
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS :

        if ( m_ui->lineEdit_irsaliye_belge_seri->text().isEmpty() EQ true ) {
            MSG_INFO(QObject::tr("İrsaliye Seri / No bilgisini girmelisiniz."),m_ui->lineEdit_irsaliye_belge_seri);
            return ADAK_FAIL;
        }
        if ( m_ui->lineEdit_belge_numarasi->text().isEmpty() EQ true ) {
            MSG_INFO(QObject::tr("İrsaliye Seri / No bilgisini girmelisiniz."),m_ui->lineEdit_belge_numarasi);
            return ADAK_FAIL;
        }
        break;

        default:
            break;
    }

    switch ( M_FIS->fis_turu ) {

        case ENUM_IRS_KONSINYE_CIKIS                     :
        case ENUM_IRS_NORMAL_CIKIS                       :
        case ENUM_IRS_NORMAL_GIRIS                       :
        case ENUM_IRS_KONSINYE_GIRIS                     :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS  :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS :
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS   :
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS  :

            if ( M_FIS->cari_hesap_id < 1 ) {
                MSG_WARNING ( tr("cari hesap seçimi yapılamış"), m_ui->searchEdit_cari_hesap_kodu );
                return ADAK_FAIL;
            }
            break;

        default :
            break;
    }

    switch ( M_FIS->fis_turu ) {

        case ENUM_STK_DEPO_TRANSFER_GIRIS   :
        case ENUM_STK_DEPO_TRANSFER_CIKIS   :

            if ( m_ui->searchEdit_alan_depo_kodu->GET_TEXT().isEmpty() EQ true ) {
                MSG_WARNING(  tr("Alan depo kodu boş bırakılamaz"), m_ui->searchEdit_alan_depo_kodu );

                return ADAK_FAIL;
            }
            if ( M_FIS->alan_depo_id < 1 ) {
                MSG_WARNING(  tr("Alan depo seçimi yapılmamış"), m_ui->searchEdit_depo_kodu );

                return ADAK_FAIL;
            }
            break;


        default :
            break;
    }

    if ( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
        if ( M_FIS->iade_edilen_irs_id < 1 ) {
            MSG_WARNING(  tr("İade edilecek irsaliye seçimi yapılmamış"), NULL );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int STOK_FISI::CHECK_FIS_RECORD_ADD()
{
    SQL_QUERY query ( DB );

    if ( QString ( m_ui->lineEdit_fis_no->text().at(0)) NE "*" ) {
        int fis_no = m_ui->lineEdit_fis_no->text().toInt();


        query.PREPARE_SELECT("stk_fisler","fis_id",
                             "fis_no = :fis_no AND fis_tarihi = :fis_tarihi");

        query.SET_VALUE(":fis_no" ,fis_no );
        query.SET_VALUE(":fis_tarihi" ,m_ui->adakDate_fis_tarihi->DATE());

        if ( query.SELECT() > 0 ) {
            MSG_WARNING(  tr("Aynı numarada başka bir fiş var. Fiş numarasını değiştiriniz."), m_ui->lineEdit_fis_no );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() NE true ) {
        if( M_FIS->irsaliye_alis_satis_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI ) {
            if( BELGE_SERI_KONTROLU( m_ui->lineEdit_irsaliye_belge_seri->text(), m_ui->lineEdit_belge_numarasi->text(), IRSALIYE_MODULU, M_FIS->cari_hesap_id ) EQ false ) {
                int secim = MSG_YES_NO( "Ayni Belge Seri + Belge Numarasi kayıtlı!. Devam Etmek istermisiniz ?", m_ui->lineEdit_irsaliye_belge_seri );
                if( secim EQ ADAK_NO ) {
                    return ADAK_FAIL;
                }
            }
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_FISI::ADD_FIS_RECORD
***************************************************************************************/

int STOK_FISI::ADD_FIS_RECORD()
{
    int fis_no = 0 ;

    if ( QString ( m_ui->lineEdit_fis_no->text().at(0)) EQ "*") {
        fis_no = STK_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_fis_tarihi->DATE());
    }
    else {
        fis_no = m_ui->lineEdit_fis_no->text().toInt();
    }

    int modul_id = -1;
    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_STOK_FISI ) {
        modul_id = STOK_MODULU;
    }
    else if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {
        modul_id = IRSALIYE_MODULU;
    }

    M_FIS->fis_no                       = fis_no;
    M_FIS->fis_tarihi                   = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->aciklama                     = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->doviz_tutar_bilgileri        = m_ui->textEdit_doviz_tutar_bilgileri->toPlainText();
    M_FIS->sevk_tarihi                  = m_ui->adakDate_sevk_tarihi->DATE();
    M_FIS->belge_seri                   = m_ui->lineEdit_irsaliye_belge_seri->text();
    M_FIS->belge_numarasi               = m_ui->lineEdit_belge_numarasi->text();
    M_FIS->cari_hesap_ismi              = m_ui->lineEdit_cari_hesap_ismi->text();
    M_FIS->vergi_dairesi                = m_ui->comboBox_vergi_dairesi->currentText();
    M_FIS->vergi_numarasi               = m_ui->lineEdit_vergi_no->text();
    M_FIS->teslim_adresi                = m_ui->limitedTextEdit_adres->toPlainText();
    M_FIS->kdv_oran_bilgileri           = m_ui->textEdit_kdv_yuzdeleri->toPlainText();
    M_FIS->otv_oran_bilgileri           = m_ui->textEdit_otv_yuzdeleri->toPlainText();
    M_FIS->irsaliye_faturalastirildi_mi = 0;
    M_FIS->iptal_mi                     = 0;
    M_FIS->modul_id                     = modul_id;
    M_FIS->program_id                   = E9_PROGRAMI;
    M_FIS->irsaliye_alis_satis_turu     = M_FIS->irsaliye_alis_satis_turu;
    M_FIS->kdv_muaf_mi                  = M_STOK_DETAYLARI->kdv_muaf_mi;
    M_FIS->muh_fis_id                   = 0;
    M_FIS->base_fis_id                  = 0;
    M_FIS->muh_hesap_id                 = M_STOK_DETAYLARI->muh_hesap_id;
    M_FIS->otv_muaf_mi                  = M_STOK_DETAYLARI->otv_muaf_mi;
    M_FIS->doviz_kuru                   = m_ui->commaEdit_kur->GET_DOUBLE();
    M_FIS->parite                       = m_ui->commaEdit_parite->GET_DOUBLE();
    M_FIS->iskonto_yuzdesi              = m_ui->commaEdit_iskonto_yuzdesi->GET_DOUBLE();
    M_FIS->iskonto_tutari               = m_ui->commaEdit_iskonto_tutari->GET_DOUBLE();
    M_FIS->stk_irs_para_birimi_id       = DVZ_GET_DOVIZ_ID( m_ui->comboBox_stk_irs_para_birimi_id->currentText() );

    int fis_id = STK_IRS_FIS_EKLE( M_FIS );

    m_ui->lineEdit_fis_no->setText(QVariant(fis_no).toString());

    m_log_kaydi_eklenecek_mi = true;
    m_kayit_eklendi          = true;

    m_log_detaylari = STK_GET_STOK_FIS_TURU(M_FIS->fis_turu) + "\n";


    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {
        m_log_detaylari.append("İrsaliye Tarihi : " + m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM yyyy dddd")
                               + " , Sevk Tarihi : " + m_ui->adakDate_sevk_tarihi->QDATE().toString("dd MMMM yyyy dddd")+
                               "\nFiş No : " + QVariant(fis_no).toString() + " , İrsaliye : " +
                               m_ui->lineEdit_irsaliye_belge_seri->text() + " " + m_ui->lineEdit_belge_numarasi->text() );

        if ( M_FIS->fis_turu EQ ENUM_STK_DEPO_TRANSFER_CIKIS OR M_FIS->fis_turu EQ ENUM_STK_DEPO_TRANSFER_GIRIS ) {
            m_log_detaylari.append("\nGönderen Depo : " + m_ui->searchEdit_depo_kodu->GET_TEXT() +
                                   "  " + m_ui->lineEdit_depo_adi->text() + ",Alan Depo : " +
                                   m_ui->searchEdit_alan_depo_kodu->GET_TEXT() + "  " +
                                   m_ui->lineEdit_alan_depo_adi->text());
        }
        else {
            m_log_detaylari.append(+"\nDepo Kodu / Adı :"
                                   + m_ui->searchEdit_depo_kodu->GET_TEXT() + "  " + m_ui->lineEdit_depo_adi->text()
                                   +",Cari Hesap Kodu / Adı : " + m_ui->searchEdit_cari_hesap_kodu->GET_TEXT() +
                                   "  " + m_ui->lineEdit_cari_hesap_ismi->text());
        }

    }
    else {
        m_log_detaylari.append("Fiş Tarihi : " + m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM yyyy dddd")
                               + " , Fiş No : " + QVariant(fis_no).toString() + "Depo Kodu  / Adı :"
                               + m_ui->searchEdit_depo_kodu->GET_TEXT() + "  " + m_ui->lineEdit_depo_adi->text());
    }

    return fis_id;
}

/**************************************************************************************
                   STOK_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int STOK_FISI::CHECK_FIS_RECORD_UPDATE ( int p_fis_id )
{

    if ( STOK_FISINDE_DEGISIKLIK_YAPILABILIR_MI( p_fis_id ) EQ false ) {
        return ADAK_FAIL;
    }

    m_iade_irsaliyesi_degistirildi_mi = false;

    SQL_QUERY query  ( DB );

    m_fis_tarihi_degistirildi_mi = false;

    if ( QString ( m_ui->lineEdit_fis_no->text().at(0)) EQ "*" ) {
        MSG_WARNING(  tr("Fiş numarasını güncellemek için fiş numarası kısmına sadece sayı girebilirsiniz."), m_ui->lineEdit_fis_no );

        return ADAK_FAIL;
    }

    int fis_no = m_ui->lineEdit_fis_no->text().toInt();

    query.PREPARE_SELECT ( "stk_fisler","fis_id",
                           "fis_no = :fis_no AND fis_tarihi = :fis_tarihi");

    query.SET_VALUE ( ":fis_no",  fis_no );
    query.SET_VALUE ( ":fis_tarihi",    m_ui->adakDate_fis_tarihi->DATE());

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        if ( query.VALUE(0).toInt() NE p_fis_id ) {
            MSG_WARNING(  tr("Aynı numarada başka bir fiş var. Fiş numarasını değiştiriniz."), m_ui->lineEdit_fis_no );

            return ADAK_FAIL;
        }
    }

    if ( M_FIS->depo_id < 1 ) {
        MSG_WARNING(  tr("Önce depo seçimi yapmalısınız."), m_ui->searchEdit_depo_kodu );

        return ADAK_FAIL;
    }


    query.PREPARE_SELECT ( "stk_fisler","fis_tarihi, depo_id, alan_depo_id, iade_edilen_irs_id ",
                           "fis_id = :fis_id");
    query.SET_VALUE(":fis_id", p_fis_id );

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr("Fiş bilgileri okunamadı."), NULL );
        return ADAK_FAIL;
    }

    query.NEXT();

    int     old_iade_edilen_irs_id  = query.VALUE("iade_edilen_irs_id").toInt();

    QDate   old_fis_tarihi          = QDate::fromString ( query.VALUE(0).toString(),"yyyy.MM.dd" );
    QDate   new_fis_tarihi          = m_ui->adakDate_fis_tarihi->QDATE();

    int     depo_id                 = 0;

    QString depo_kodu;
    QString depo_adi;


    switch ( M_FIS->fis_turu ) {
        case ENUM_STK_DEPO_TRANSFER_CIKIS :
        default                      : {

            depo_id = query.VALUE(1).toInt();

            SUBE_GET_DEPO_KODU_ADI( depo_id, &depo_kodu, &depo_adi );

            if ( depo_id NE M_FIS->depo_id ) {
                MSG_WARNING(  tr("Bu irsaliye / stok fişi için depo kodu: " + depo_kodu.toUtf8() + ", depo_adı: " + depo_adi.toUtf8()  + " olan depo kaydedilmiş. Depo değişikliği yapamazsınız." ), NULL );
                return ADAK_FAIL;
            }

            break;
        }
        case ENUM_STK_DEPO_TRANSFER_GIRIS : {

            depo_id = query.VALUE(2).toInt();

            SUBE_GET_DEPO_KODU_ADI(depo_id, &depo_kodu, &depo_adi );



            if ( depo_id NE M_FIS->alan_depo_id ) {
                MSG_WARNING(  tr("Bu irsaliye / stok fişi için depo kodu: " + depo_kodu.toUtf8() + ", depo_adı: " + depo_adi.toUtf8()  + " olan depo kaydedilmiş. Depo değişikliği yapamazsınız." ), NULL );
                return ADAK_FAIL;
            }
            break;
        }
    }

    if ( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
        query.PREPARE_SELECT ( "stk_fisler","fis_tarihi","fis_id = :fis_id");
        query.SET_VALUE      ( ":fis_id", M_FIS->iade_edilen_irs_id );
        if ( query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr("İade irsaliyesi bilgileri okunamadı"), NULL );
            return ADAK_FAIL;
        }
        query.NEXT();

        QDate iade_edilen_irsaliye_tarihi = QDate::fromString ( query.VALUE(0).toString(), "yyyy.MM.dd" );

        if ( ( iade_edilen_irsaliye_tarihi > m_ui->adakDate_fis_tarihi->QDATE() ) OR ( iade_edilen_irsaliye_tarihi < m_ui->adakDate_fis_tarihi->QDATE().addDays ( -m_irs_iade_edilecek_gun_sayisi ) ) ) {
            MSG_WARNING(  tr("İrsaliye tarihini, iade edilecek irsaliyenin tarihinden daha geriye veya iade edilecek irsaliye tarihinden %n günden daha ileriye alamazsınız", "", m_irs_iade_edilecek_gun_sayisi ), NULL );
            return ADAK_FAIL;
        }

        if ( old_iade_edilen_irs_id NE M_FIS->iade_edilen_irs_id ) {
            query.PREPARE_SELECT ( "stk_fis_satirlari","fis_satiri_id","fis_id = :fis_id");
            query.SET_VALUE      ( ":fis_id", p_fis_id );
            if ( query.SELECT() > 0 ) {
                MSG_WARNING(  tr("Önceki seçilen iade irsaliyesinden kayıtlı satırlar var. Bu satırları silmeden seçili iade irsaliyesini değiştiremezsiniz."), NULL);
                return ADAK_FAIL;
            }
            m_iade_irsaliyesi_degistirildi_mi = true;
        }

    }

    if ( old_fis_tarihi NE new_fis_tarihi ) {
        m_fis_tarihi_degistirildi_mi = true;

        int irsaliye_stok_fisi_alis_satis_turu = IRSALIYE_ALIS_SATIS_TURUNU_BELIRLE ();


        if ( STK_IRSALIYE_TARIH_DEGISIKLIGINE_GORE_STOK_MIKTARLARI_KONTROLU ( p_fis_id, old_fis_tarihi,
                                                                              new_fis_tarihi, depo_id,
                                                                              irsaliye_stok_fisi_alis_satis_turu,
                                                                              this )  EQ false ) {
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void STOK_FISI::UPDATE_FIS_RECORD ( int p_fis_id )
{
    SQL_QUERY    query  ( DB );

    int fis_no = m_ui->lineEdit_fis_no->text().toInt();

    query.PREPARE_SELECT ( "stk_fisler","fis_tarihi, depo_id, alan_depo_id, iskonto_yuzdesi,",
                           "fis_id = :fis_id" );

    query.SET_VALUE(":fis_id", p_fis_id );

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr("Fiş bilgileri okunamadı."), NULL );
        return;
    }

    query.NEXT();

    int   depo_id        = 0;

    switch ( M_FIS->fis_turu ) {

        case ENUM_STK_DEPO_TRANSFER_CIKIS :
        default                           :
            depo_id = query.VALUE ( "depo_id" ).toInt();
            break;

        case ENUM_STK_DEPO_TRANSFER_GIRIS :
            depo_id = query.VALUE ( "alan_depo_id" ).toInt();
            break;
    }

    int irsaliye_stok_fisi_alis_satis_turu = IRSALIYE_ALIS_SATIS_TURUNU_BELIRLE ();

    QDate   old_fis_tarihi          =   QDate::fromString ( query.VALUE ( "fis_tarihi" ).toString(), "yyyy.MM.dd" );
    QDate   new_fis_tarihi          =   m_ui->adakDate_fis_tarihi->QDATE();
    double  old_iskonto_yuzdesi     =   query.VALUE ( "iskonto_yuzdesi" ).toDouble ();
    double  new_iskonto_yuzdesi     =   m_ui->commaEdit_iskonto_yuzdesi->GET_DOUBLE   ();

    bool    iskonto_yuzdesi_degisti = false;

    if ( old_iskonto_yuzdesi NE new_iskonto_yuzdesi ) {
        iskonto_yuzdesi_degisti = true;
    }

    if ( m_fis_tarihi_degistirildi_mi EQ true ) {

        STK_IRSALIYE_TARIH_DEGISIKLIGINE_GORE_STOK_GIRIS_CIKISLARINI_DUZENLE ( p_fis_id, old_fis_tarihi, new_fis_tarihi,
                                                                               depo_id, irsaliye_stok_fisi_alis_satis_turu);

    }

    query.PREPARE_UPDATE ( "stk_fisler ","fis_id",
                           "fis_no                          ,"
                           "fis_tarihi                      ,"
                           "belge_seri                      ,"
                           "belge_numarasi                  ,"
                           "aciklama                        ,"
                           "doviz_tutar_bilgileri           ,"
                           "irsaliye_mi_stok_fisi_mi        ,"
                           "fis_turu                        ,"
                           "depo_id                         ,"
                           "alan_depo_id                    ,"
                           "sevk_tarihi                     ,"
                           "cari_hesap_id                   ,"
                           "cari_hesap_ismi                 ,"
                           "vergi_dairesi                   ,"
                           "vergi_numarasi                  ,"
                           "teslim_adresi                   ,"
                           "ara_toplam                      ,"
                           "isk_sonrasi_otv_tutari          ,"
                           "isk_sonrasi_otv_dahil_tutar     ,"
                           "isk_sonrasi_kdv_tutari          ,"
                           "irsaliye_tutari                 ,"
                           "iskonto_yuzdesi                 ,"
                           "iskonto_tutari                  ,"
                           "kdv_oran_bilgileri              ,"
                           "otv_oran_bilgileri              ,"
                           "iade_edilen_irs_id              ,"
                           "kdv_muaf_mi                     ,"
                           "otv_muaf_mi                     ,"
                           "muh_hesap_id                    ,"
                           "stk_irs_para_birim_id           ,"
                           "parite                          ,"
                           "doviz_kuru                       ",
                           "fis_id = :fis_id " );

    query.SET_VALUE ( ":fis_no"                         , m_ui->lineEdit_fis_no->text().toInt()                             );
    query.SET_VALUE ( ":fis_tarihi"                     , m_ui->adakDate_fis_tarihi->DATE()                                );
    query.SET_VALUE ( ":belge_seri"                     , m_ui->lineEdit_irsaliye_belge_seri->text()                        );
    query.SET_VALUE ( ":belge_numarasi"                 , m_ui->lineEdit_belge_numarasi->text()                             );
    query.SET_VALUE ( ":aciklama"                       , m_ui->limitedTextEdit_aciklama->toPlainText()                     );
    query.SET_VALUE ( ":doviz_tutar_bilgileri"          , m_ui->textEdit_doviz_tutar_bilgileri->toPlainText()               );
    query.SET_VALUE ( ":irsaliye_mi_stok_fisi_mi"       , M_FIS->irsaliye_mi_stok_fisi_mi                                   );
    query.SET_VALUE ( ":fis_turu"                       , M_FIS->fis_turu                                                   );
    query.SET_VALUE ( ":depo_id"                        , M_FIS->depo_id                                                    );
    query.SET_VALUE ( ":alan_depo_id"                   , M_FIS->alan_depo_id                                               );
    query.SET_VALUE ( ":sevk_tarihi"                    , m_ui->adakDate_sevk_tarihi->DATE()                               );
    query.SET_VALUE ( ":cari_hesap_id"                  , M_FIS->cari_hesap_id                                              );
    query.SET_VALUE ( ":cari_hesap_ismi"                , m_ui->lineEdit_cari_hesap_ismi->text()                            );
    query.SET_VALUE ( ":vergi_dairesi"                  , m_ui->comboBox_vergi_dairesi->currentText()                       );
    query.SET_VALUE ( ":vergi_numarasi"                 , m_ui->lineEdit_vergi_no->text()                                   );
    query.SET_VALUE ( ":teslim_adresi"                  , m_ui->limitedTextEdit_adres->toPlainText()                        );
    query.SET_VALUE ( ":ara_toplam"                     , m_ui->commaEdit_ara_toplam->GET_DOUBLE()                          );
    query.SET_VALUE ( ":isk_sonrasi_otv_tutari"         , m_ui->commaEdit_isk_sonrasi_otv_tutari->GET_DOUBLE()              );
    query.SET_VALUE ( ":isk_sonrasi_otv_dahil_tutar"    , m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->GET_DOUBLE()         );
    query.SET_VALUE ( ":isk_sonrasi_kdv_tutari"         , m_ui->commaEdit_isk_sonrasi_kdv_tutari->GET_DOUBLE()              );
    query.SET_VALUE ( ":irsaliye_tutari"                , m_ui->commaEdit_irsaliye_tutari->GET_DOUBLE()                     );
    query.SET_VALUE ( ":iskonto_yuzdesi"                , m_ui->commaEdit_iskonto_yuzdesi->GET_DOUBLE()                     );
    query.SET_VALUE ( ":iskonto_tutari"                 , m_ui->commaEdit_iskonto_tutari->GET_DOUBLE()                      );
    query.SET_VALUE ( ":kdv_oran_bilgileri"             , m_ui->textEdit_kdv_yuzdeleri->toPlainText()                       );
    query.SET_VALUE ( ":otv_oran_bilgileri"             , m_ui->textEdit_otv_yuzdeleri->toPlainText()                       );
    query.SET_VALUE ( ":iade_edilen_irs_id"             , M_FIS->iade_edilen_irs_id                                         );
    query.SET_VALUE ( ":kdv_muaf_mi"                    , M_STOK_DETAYLARI->kdv_muaf_mi                                     );
    query.SET_VALUE ( ":otv_muaf_mi"                    , M_STOK_DETAYLARI->otv_muaf_mi                                     );
    query.SET_VALUE ( ":muh_hesap_id"                   , M_STOK_DETAYLARI->muh_hesap_id                                    );
    query.SET_VALUE ( ":stk_irs_para_birim_id"          , M_FIS->stk_irs_para_birimi_id                                     );
    query.SET_VALUE ( ":parite"                         , m_ui->commaEdit_parite->GET_DOUBLE()                              );
    query.SET_VALUE ( ":doviz_kuru"                     , m_ui->commaEdit_kur->GET_DOUBLE()                                 );
    query.SET_VALUE ( ":fis_id"                         , p_fis_id                                                          );


    query.UPDATE    ();

    m_ui->lineEdit_fis_no->setText ( QVariant ( fis_no ).toString() );

    if ( iskonto_yuzdesi_degisti EQ true ) {

        for ( int row_number = 0; row_number < m_ui->tableWidget_fis_satirlari->rowCount() -1; row_number++ ) {

            QCommaEdit  *   commaEdit_toplu_iskonto_yuzdesi           = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, TOPLU_ISKONTO_YUZDESI_COLUMN          );
            QCommaEdit  *   commaEdit_t_is_sonrasi_birim_fiyat        = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, T_IS_SONRASI_BIRIM_FIYAT_COLUMN       );
            QCommaEdit  *   commaEdit_t_is_sonrasi_satir_tutari       = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, T_IS_SONRASI_SATIR_TUTARI_COLUMN      );
            QCommaEdit  *   commaEdit_t_is_sonrasi_vergi_haric_tutar  = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN );
            QCommaEdit  *   commaEdit_t_is_sonrasi_otv_tutari         = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, T_IS_SONRASI_OTV_TUTARI_COLUMN        );
            QCommaEdit  *   commaEdit_t_is_sonrasi_otv_dahil_tutar    = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN   );
            QCommaEdit  *   commaEdit_t_is_sonrasi_kdv_tutari         = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, T_IS_SONRASI_KDV_TUTARI_COLUMN        );
            QCommaEdit  *   commaEdit_t_is_sonrasi_kdv_dahil_tutar    = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( row_number, T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN   );


            int fis_satiri_id = m_ui->tableWidget_fis_satirlari->item ( row_number, ROW_ID_COLUMN )->text().toInt();

            query.PREPARE_UPDATE ( "stk_fis_satirlari ", "fis_satiri_id",
                                   "toplu_iskonto_yuzdesi           ,"
                                   "t_is_son_birim_fiyati               ,"
                                   "t_is_son_satir_tutari               ,"
                                   "t_is_son_str_vergi_haric_tutar      ,"
                                   "t_is_son_satir_otv_tutari           ,"
                                   "t_is_son_satir_otv_dahil_tutar      ,"
                                   "t_is_son_satir_kdv_tutari           ,"
                                   "t_is_son_satir_kdv_dahil_tutar       ",
                                   "fis_satiri_id = :fis_satiri_id " );

            query.SET_VALUE ( ":toplu_iskonto_yuzdesi",             commaEdit_toplu_iskonto_yuzdesi->GET_DOUBLE()                       );
            query.SET_VALUE ( ":t_is_son_birim_fiyati",             commaEdit_t_is_sonrasi_birim_fiyat->GET_DOUBLE()                    );
            query.SET_VALUE ( ":t_is_son_satir_tutari",             commaEdit_t_is_sonrasi_satir_tutari->GET_DOUBLE()                   );
            query.SET_VALUE ( ":t_is_son_str_vergi_haric_tutar",    commaEdit_t_is_sonrasi_vergi_haric_tutar->GET_DOUBLE()              );
            query.SET_VALUE ( ":t_is_son_satir_otv_tutari",         commaEdit_t_is_sonrasi_otv_tutari->GET_DOUBLE()                     );
            query.SET_VALUE ( ":t_is_son_satir_otv_dahil_tutar",    commaEdit_t_is_sonrasi_otv_dahil_tutar->GET_DOUBLE()                );
            query.SET_VALUE ( ":t_is_son_satir_kdv_tutari",         commaEdit_t_is_sonrasi_kdv_tutari->GET_DOUBLE()                     );
            query.SET_VALUE ( ":t_is_son_satir_kdv_dahil_tutar",    commaEdit_t_is_sonrasi_kdv_dahil_tutar->GET_DOUBLE()                );
            query.SET_VALUE ( ":fis_satiri_id",                     fis_satiri_id );

            query.UPDATE    ();
        }
    }

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = STK_GET_STOK_FIS_TURU(M_FIS->fis_turu) + "\n";


    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {

        m_log_detaylari.append("İrsaliye Tarihi : " + m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM yyyy dddd")
                               + " , Sevk Tarihi : " + m_ui->adakDate_sevk_tarihi->QDATE().toString("dd MMMM yyyy dddd")+
                               "\nFiş No : " + QVariant(fis_no).toString() + " , İrsaliye : " + m_ui->lineEdit_irsaliye_belge_seri->text() + " " + m_ui->lineEdit_belge_numarasi->text() );


        if ( M_FIS->fis_turu EQ ENUM_STK_DEPO_TRANSFER_CIKIS OR M_FIS->fis_turu EQ ENUM_STK_DEPO_TRANSFER_GIRIS ) {
            m_log_detaylari.append("\nGönderen Depo : " + m_ui->searchEdit_depo_kodu->GET_TEXT() +
                                   "  " + m_ui->lineEdit_depo_adi->text() + ",Alan Depo : " +
                                   m_ui->searchEdit_alan_depo_kodu->GET_TEXT() + "  " +
                                   m_ui->lineEdit_alan_depo_adi->text());
        }
        else {
            m_log_detaylari.append(+"\nDepo Kodu / Adı :"
                                   + m_ui->searchEdit_depo_kodu->GET_TEXT() + "  " + m_ui->lineEdit_depo_adi->text()
                                   +",Cari Hesap Kodu / Adı : " + m_ui->searchEdit_cari_hesap_kodu->GET_TEXT() +
                                   "  " + m_ui->lineEdit_cari_hesap_ismi->text());
        }
    }
    else {
        m_log_detaylari.append("Fiş Tarihi : " + m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM yyyy dddd")
                               + " , Fiş No : " + QVariant(fis_no).toString() + "Depo Kodu  / Adı :"
                               + m_ui->searchEdit_depo_kodu->GET_TEXT() + "  " + m_ui->lineEdit_depo_adi->text());
    }

    m_log_detaylari.append ( "\nAra Toplam : "         + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_ara_toplam->GET_DOUBLE()                  ).toString(), 2 )
                            + " , Kdv Tutarı : "       + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_kdv_tutari->GET_DOUBLE()      ).toString(), 2 )
                            + " , İrsaliye Tutarı : "  + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_irsaliye_tutari->GET_DOUBLE()             ).toString(), 2 )
                            + "\nÖtv Tutarı : "        + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_tutari->GET_DOUBLE()      ).toString(), 2 )
                            + " , Ötv Dahil Toplam : " + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->GET_DOUBLE() ).toString(), 2 ) );

}

/**************************************************************************************
                   STOK_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int STOK_FISI::CHECK_FIS_RECORD_DELETE ( int p_fis_id )
{

    switch( M_FIS->fis_turu ) {
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS:
            break;

        default:
            if ( STOK_FISINDE_DEGISIKLIK_YAPILABILIR_MI( p_fis_id, 1 ) EQ false ) {
                return ADAK_FAIL;
            }
    }

    int msg_secim = -1;

    if ( M_FIS->yazdirildi_mi EQ 1 ) {
        msg_secim =  MSG_YES_NO( tr ( "Stok fişi / İrsaliye yazdırıldığı için silinemez, iptal işlemi gerçekleştirilecektir.\n "
                           "Devam etmek istiyor musunuz?" ), NULL );
    }

    if ( msg_secim EQ ADAK_NO ) {
        return ADAK_FAIL;
    }

    int irsaliye_stok_fisi_alis_satis_turu = IRSALIYE_ALIS_SATIS_TURUNU_BELIRLE ();

    switch ( irsaliye_stok_fisi_alis_satis_turu ) {

        case ENUM_ALIS_IRSALIYESI_FATURASI :

            if ( STK_IRSALIYE_SILINMESI_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU ( p_fis_id, this ) EQ false ) {
                return ADAK_FAIL;
            }

        default :
            break;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void STOK_FISI::DELETE_FIS_RECORD ( int p_fis_id )
{
    STK_IRS_FIS_STRUCT * FIS = new STK_IRS_FIS_STRUCT;

    STK_IRS_CLEAR_FIS_STRUCT( FIS );

    STK_IRS_FIS_BILGILERINI_OKU( p_fis_id, FIS );

    FIS->irsaliye_alis_satis_turu = M_FIS->irsaliye_alis_satis_turu;

    STK_IRS_FIS_SIL( p_fis_id, FIS );


    m_log_kaydi_eklenecek_mi = true;
    m_kayit_silindi          = true;

    m_log_detaylari = STK_GET_STOK_FIS_TURU(M_FIS->fis_turu) + "\n";

    QString gonderen_depo_kodu;
    QString gonderen_depo_adi;
    QString alan_depo_kodu;
    QString alan_depo_adi;

    SUBE_GET_DEPO_KODU_ADI ( FIS->depo_id       , &gonderen_depo_kodu , &gonderen_depo_adi);
    SUBE_GET_DEPO_KODU_ADI ( FIS->alan_depo_id  , &alan_depo_kodu     , &alan_depo_adi);

    QString cari_hesap_kodu, cari_hesap_ismi;
    CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( FIS->cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );

    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {
        m_log_detaylari.append("İrsaliye Tarihi : " + QDate::fromString( FIS->fis_tarihi ).toString("dd MMMM yyyy dddd")
                               + " , Sevk Tarihi : " + QDate::fromString( FIS->sevk_tarihi ).toString("dd MMMM yyyy dddd")+
                               "\nFiş No : " + QVariant( FIS->fis_no).toString() + " , İrsaliye : " + FIS->belge_seri + " " + FIS->belge_numarasi );

        if ( M_FIS->fis_turu EQ ENUM_STK_DEPO_TRANSFER_CIKIS OR M_FIS->fis_turu EQ ENUM_STK_DEPO_TRANSFER_GIRIS ) {
            m_log_detaylari.append("\nGönderen Depo : " + gonderen_depo_kodu + "  " + gonderen_depo_adi + ",Alan Depo : " +
                                   alan_depo_kodu + "  " + alan_depo_adi);
        }
        else {
            m_log_detaylari.append(+"\nDepo Kodu / Adı :" + gonderen_depo_kodu + "  " + gonderen_depo_adi
                                   +",Cari Hesap Kodu / Adı : " + cari_hesap_kodu + "  " + cari_hesap_ismi);
        }
    }
    else {
        m_log_detaylari.append("Fiş Tarihi : " + QDate::fromString( FIS->fis_tarihi ).toString("dd MMMM yyyy dddd")
                               + " , Fiş No : " + QVariant( FIS->fis_no).toString() + "Depo Kodu  / Adı :" + gonderen_depo_kodu + "  " + gonderen_depo_adi);
    }

    m_log_detaylari.append ( "\nAra Toplam : "        + VIRGUL_EKLE ( QVariant ( ROUND ( FIS->ara_toplam, -2 ) ).toString(), 2 )
                           + " , Kdv Tutarı : "       + VIRGUL_EKLE ( QVariant ( ROUND ( FIS->isk_sonrasi_kdv_tutari ) ).toString(), 2 )
                           + " , İrsaliye Tutarı : "  + VIRGUL_EKLE ( QVariant ( ROUND ( FIS->irsaliye_tutari, -2 ) ).toString(), 2 )
                           + "\nÖtv Tutarı : "        + VIRGUL_EKLE ( QVariant ( ROUND ( FIS->isk_sonrasi_otv_tutari ) ).toString(), 2 )
                           + " , Ötv Dahil Toplam : " + VIRGUL_EKLE ( QVariant ( ROUND ( FIS->isk_sonrasi_otv_dahil_tutar ) ).toString(), 2 ) );

}

/**************************************************************************************
                   STOK_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int STOK_FISI::SELECT_FIS_RECORD()
{
    return OPEN_STOK_FISI_ARAMA ( M_FIS->irsaliye_alis_satis_turu, M_FIS->irsaliye_mi_stok_fisi_mi, M_FIS->iade_irsaliyesi_mi, M_FIS->fis_turu, this );
}

/**************************************************************************************
                   STOK_FISI::FIND_FIS_RECORD
***************************************************************************************/

int STOK_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY query ( DB );

    QString query_str = "irsaliye_mi_stok_fisi_mi = :irsaliye_mi_stok_fisi_mi "
                        "AND fis_no = :fis_no "
                        "AND fis_tarihi   = :fis_tarihi";

    query.PREPARE_SELECT( "stk_fisler", "fis_id", query_str );

    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ){
        query.AND_EKLE   ( " iade_irsaliyesi_mi = :iade_irsaliyesi_mi ");
        query.SET_VALUE  ( ":iade_irsaliyesi_mi", M_FIS->iade_irsaliyesi_mi );
    }

    query.SET_VALUE ( ":irsaliye_mi_stok_fisi_mi" , M_FIS->irsaliye_mi_stok_fisi_mi );
    query.SET_VALUE ( ":fis_no"             , m_ui->lineEdit_fis_no->text().toInt() );
    query.SET_VALUE ( ":fis_tarihi"               , m_ui->adakDate_fis_tarihi->DATE() );

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   STOK_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int STOK_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY query ( DB );

    QString query_str = "irsaliye_mi_stok_fisi_mi = :irsaliye_mi_stok_fisi_mi "
                        "AND fis_turu             = :fis_turu ";

    query.PREPARE_SELECT( "stk_fisler", "fis_id", query_str );

    query.SET_VALUE ( ":irsaliye_mi_stok_fisi_mi", M_FIS->irsaliye_mi_stok_fisi_mi );
    query.SET_VALUE ( ":fis_turu", M_FIS->fis_turu );

    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ){
        query.AND_EKLE   ( " iade_irsaliyesi_mi = :iade_irsaliyesi_mi ");
        query.SET_VALUE  ( ":iade_irsaliyesi_mi", M_FIS->iade_irsaliyesi_mi );
    }

    if ( query.SELECT( "fis_tarihi, fis_no", 0 , 1) EQ 0 ) {
        return 0;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   STOK_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int STOK_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY query ( DB );

    QString query_str = "irsaliye_mi_stok_fisi_mi = :irsaliye_mi_stok_fisi_mi "
                        "AND fis_turu                   = :fis_turu ";

    query.PREPARE_SELECT( "stk_fisler", "fis_id", query_str );

    query.SET_VALUE ( ":irsaliye_mi_stok_fisi_mi", M_FIS->irsaliye_mi_stok_fisi_mi );
    query.SET_VALUE ( ":fis_turu"                , M_FIS->fis_turu );

    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ){
        query.AND_EKLE   ( " iade_irsaliyesi_mi = :iade_irsaliyesi_mi ");
        query.SET_VALUE  ( ":iade_irsaliyesi_mi", M_FIS->iade_irsaliyesi_mi );
    }

    if ( query.SELECT(  "fis_tarihi DESC, fis_no DESC", 0,1 ) EQ 0 ) {
        return 0;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   STOK_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int STOK_FISI::FIND_PREV_FIS_RECORD ()
{
    SQL_QUERY query ( DB );

    QString query_str = "fis_no           < :fis_no "
                        "AND irsaliye_mi_stok_fisi_mi = :irsaliye_mi_stok_fisi_mi "
                        "AND fis_turu                 = :fis_turu "
                        "AND fis_tarihi               = :fis_tarihi";

    query.PREPARE_SELECT( "stk_fisler", "fis_id", query_str );

    query.SET_VALUE ( ":fis_no"             , m_ui->lineEdit_fis_no->text().toInt());
    query.SET_VALUE ( ":irsaliye_mi_stok_fisi_mi" , M_FIS->irsaliye_mi_stok_fisi_mi );
    query.SET_VALUE ( ":fis_turu"                 , M_FIS->fis_turu );
    query.SET_VALUE ( ":fis_tarihi"              , m_ui->adakDate_fis_tarihi->DATE());

    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ){
        query.AND_EKLE   ( " iade_irsaliyesi_mi = :iade_irsaliyesi_mi ");
        query.SET_VALUE  ( ":iade_irsaliyesi_mi", M_FIS->iade_irsaliyesi_mi );
    }

    if ( query.SELECT("fis_no DESC", 0 ,1 ) > 0 ) {
        query.NEXT();
        return query.VALUE(0).toInt();
    }

    query_str = "irsaliye_mi_stok_fisi_mi = :irsaliye_mi_stok_fisi_mi "
                 "AND fis_turu                   = :fis_turu "
                 "AND fis_tarihi                 < :fis_tarihi";

    query.PREPARE_SELECT( "stk_fisler", "fis_id", query_str );


    query.SET_VALUE ( ":irsaliye_mi_stok_fisi_mi" , M_FIS->irsaliye_mi_stok_fisi_mi );
    query.SET_VALUE ( ":fis_turu"                 , M_FIS->fis_turu );
    query.SET_VALUE ( ":fis_tarihi"              , m_ui->adakDate_fis_tarihi->DATE());

    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ){
        query.AND_EKLE   ( " iade_irsaliyesi_mi = :iade_irsaliyesi_mi ");
        query.SET_VALUE  ( ":iade_irsaliyesi_mi", M_FIS->iade_irsaliyesi_mi );
    }

    if ( query.SELECT( "fis_tarihi DESC, fis_no DESC" , 0,1 ) EQ 0 ) {
        return 0;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   STOK_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int STOK_FISI::FIND_NEXT_FIS_RECORD ()
{
    SQL_QUERY query ( DB );

    QString query_str = "fis_no           > :fis_no "
                        "AND irsaliye_mi_stok_fisi_mi = :irsaliye_mi_stok_fisi_mi "
                        "AND fis_turu                 = :fis_turu "
                        "AND fis_tarihi               = :fis_tarihi";

    query.PREPARE_SELECT( "stk_fisler", "fis_id", query_str );

    query.SET_VALUE ( ":fis_no"                   , m_ui->lineEdit_fis_no->text().toInt());
    query.SET_VALUE ( ":irsaliye_mi_stok_fisi_mi" , M_FIS->irsaliye_mi_stok_fisi_mi );
    query.SET_VALUE ( ":fis_turu"                 , M_FIS->fis_turu );
    query.SET_VALUE ( ":fis_tarihi"               , m_ui->adakDate_fis_tarihi->DATE());

    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ){
        query.AND_EKLE   ( " iade_irsaliyesi_mi = :iade_irsaliyesi_mi ");
        query.SET_VALUE  ( ":iade_irsaliyesi_mi", M_FIS->iade_irsaliyesi_mi );
    }

    if ( query.SELECT( "fis_no", 0,1) > 0 ) {
        query.NEXT();
        return query.VALUE(0).toInt();
    }

    query_str = "irsaliye_mi_stok_fisi_mi = :irsaliye_mi_stok_fisi_mi "
                "AND fis_turu                   = :fis_turu "
                "AND fis_tarihi                 > :fis_tarihi";

    query.PREPARE_SELECT( "stk_fisler", "fis_id", query_str );

    query.SET_VALUE ( ":irsaliye_mi_stok_fisi_mi" , M_FIS->irsaliye_mi_stok_fisi_mi );
    query.SET_VALUE ( ":fis_turu"                 , M_FIS->fis_turu );
    query.SET_VALUE ( ":fis_tarihi"               , m_ui->adakDate_fis_tarihi->DATE());

    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ){
        query.AND_EKLE   ( " iade_irsaliyesi_mi = :iade_irsaliyesi_mi ");
        query.SET_VALUE  ( ":iade_irsaliyesi_mi", M_FIS->iade_irsaliyesi_mi );
    }

    if ( query.SELECT( "fis_tarihi, fis_no", 0, 1 ) EQ 0 ) {
        return 0;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   STOK_FISI::CHECK_LINE_VAR
***************************************************************************************/

int STOK_FISI::CHECK_LINE_VAR ( int p_row_number, QObject * p_object )
{
    if ( M_FIS->depo_id < 1 ) {
        MSG_WARNING(  tr("Önce depo seçimi yapmalısınız."), m_ui->searchEdit_depo_kodu );

        return ADAK_FAIL;
    }

    switch ( M_FIS->fis_turu ) {
        case ENUM_STK_DEPO_TRANSFER_GIRIS   :
        case ENUM_STK_DEPO_TRANSFER_CIKIS   :
            if ( m_ui->searchEdit_alan_depo_kodu->GET_TEXT().isEmpty() EQ true ) {
                MSG_WARNING(  tr("Alan depo kodu boş bırakılamaz"), m_ui->searchEdit_alan_depo_kodu );

                return ADAK_FAIL;
            }
            if ( M_FIS->alan_depo_id < 1 ) {
                MSG_WARNING(  tr("Alan depo seçimi yapılmamış"), m_ui->searchEdit_depo_kodu );

                return ADAK_FAIL;
            }
            break;

        default :
            break;
    }

    SQL_QUERY query ( DB );

    QLineEdit   * lineEdit_seri_no_list             = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SERI_NO_LISTE_COLUMN              );
    QLineEdit   * lineEdit_urun_id                  = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN                    );
    QSearchEdit * searchEdit_urun_kodu              = ( QSearchEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_KOD_COLUMN                   );
    QLineEdit   * lineEdit_urun_adi                 = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_AD_COLUMN                    );
    QComboBox   * comboBox_stok_birimi              = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN                );
    QCommaEdit  * commaEdit_satir_tutari            = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN               );
    QCommaEdit  * commaEdit_satir_stok_miktari      = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_MIKTARI_COLUMN               );
    QCommaEdit  * commaEdit_isk_oncesi_birim_fiyat  = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN     );
    QCommaEdit  * commaEdit_isk_sonrasi_birim_fiyat = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN    );
    QCommaEdit  * commaEdit_iskonto_yuzdesi         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISKONTO_YUZDESI_COLUMN            );
    QCommaEdit  * commaEdit_iskonto_tutari          = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISKONTO_TUTARI_COLUMN             );
    QLineEdit   * lineEdit_stok_takip_sekli         = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_TAKIP_SEKLI_COLUMN           );
    QPushButton * pushButton_seri_no_parti_kodu     = ( QPushButton * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SERI_NO_PARTI_KODU_COLUMN        );
    QLineEdit   * lineEdit_parti_id                 = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, PARTI_ID_COLUMN                   );
    //QLineEdit   * lineEdit_para_birimi              = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, PARA_BIRIMI_COLUMN                );
    QCommaEdit  * commaEdit_vergi_haric_tutar       = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTARI_COLUMN         );
    QComboBox   * comboBox_kdv_orani                = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_ORANI_COLUMN                  );
    QComboBox   * comboBox_kdv_dahil_mi             = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN               );
    QCommaEdit  * commaEdit_kdv_tutari              = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_TUTAR_COLUMN                  );
    QCommaEdit  * commaEdit_kdv_dahil_tutar         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_DAHIL_TUTARI_COLUMN           );
    QComboBox   * comboBox_otv_orani                = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_ORANI_COLUMN                  );
    QCommaEdit  * commaEdit_otv_tutari              = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_TUTARI_COLUMN                 );
    QCommaEdit  * commaEdit_otv_dahil_tutar         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_DAHIL_TUTAR_COLUMN            );

    int urun_id = 0;

    if ( p_object EQ searchEdit_urun_kodu ) {

        if ( searchEdit_urun_kodu->GET_TEXT().isEmpty() EQ true ) {

            lineEdit_urun_id->setText                   ( "-1" );
            lineEdit_urun_adi->clear                    ();
            lineEdit_urun_adi->setReadOnly              ( false );
            comboBox_stok_birimi->clear                 ();
            commaEdit_satir_tutari->clear               ();
            commaEdit_satir_stok_miktari->clear         ();
            commaEdit_isk_oncesi_birim_fiyat->clear     ();
            commaEdit_isk_sonrasi_birim_fiyat->clear    ();
            commaEdit_iskonto_yuzdesi->clear            ();
            commaEdit_iskonto_tutari->clear             ();
            lineEdit_stok_takip_sekli->setText          ( "-1" );
            pushButton_seri_no_parti_kodu->setEnabled   ( true );
            lineEdit_parti_id->setText                  ( "-1" );
            commaEdit_vergi_haric_tutar->clear          ();
            comboBox_kdv_orani->setCurrentIndex         ( 0 );
            comboBox_kdv_dahil_mi->setCurrentIndex      ( 0 );
            commaEdit_kdv_tutari->clear                 ();
            commaEdit_kdv_dahil_tutar->clear            ();
            comboBox_otv_orani->setCurrentIndex         ( 0 );
            commaEdit_otv_tutari->clear                 ();
            commaEdit_otv_dahil_tutar->clear            ();
            return ADAK_OK;
        }



        switch ( M_FIS->irsaliye_alis_satis_turu ) {

            case ENUM_SATIS_IRSALIYESI_FATURASI :

                query.PREPARE_SELECT("stk_depodaki_urunler, stk_urunler",
                                     "stk_depodaki_urunler.urun_id, urun_adi, "
                                     "stok_takip_sekli, alis_kdv_orani, alis_otv_orani, "
                                     "satis_kdv_orani, satis_otv_orani ",
                                     "stk_depodaki_urunler.urun_id = stk_urunler.urun_id "
                                     "AND depo_id        = :depo_id "
                                     "AND ( urun_kodu    = :urun_kodu "
                                     "OR barkod_numarasi = :barkod_numarasi )");
                query.SET_VALUE ( ":depo_id",           M_FIS->depo_id                        );
                query.SET_VALUE ( ":urun_kodu",         searchEdit_urun_kodu->GET_TEXT() );
                query.SET_VALUE ( ":barkod_numarasi",   searchEdit_urun_kodu->GET_TEXT() );
                if ( query.SELECT() EQ 0 ) {
                    MSG_WARNING(  tr("Seçili depoda bu ürün bulunamadı."), searchEdit_urun_kodu );

                    return ADAK_FAIL_UNDO;
                }

                break;
            case ENUM_ALIS_IRSALIYESI_FATURASI :
            default                            :

                query.PREPARE_SELECT("stk_urunler","urun_id, urun_adi, stok_takip_sekli, "
                                     "alis_kdv_orani, alis_otv_orani,"
                                     "satis_kdv_orani, satis_otv_orani ",
                                     "urun_kodu    = :urun_kodu "
                                     "OR barkod_numarasi = :barkod_numarasi" );

                query.SET_VALUE ( ":urun_kodu",       searchEdit_urun_kodu->GET_TEXT() );
                query.SET_VALUE ( ":barkod_numarasi", searchEdit_urun_kodu->GET_TEXT() );

                if ( query.SELECT() EQ 0 ) {
                    MSG_WARNING(  tr("Ürün kodu bulunamadı."), searchEdit_urun_kodu );

                    return ADAK_FAIL_UNDO;
                }
                break;
        }

        query.NEXT();

        urun_id = query.VALUE(0).toInt();

        lineEdit_urun_id->setText      ( QVariant(urun_id).toString());
        lineEdit_urun_adi->setText     ( query.VALUE(1).toString());
        lineEdit_urun_adi->setReadOnly ( true );

        int stok_takip_sekli = query.VALUE(2).toInt();
        lineEdit_stok_takip_sekli->setText(QVariant(stok_takip_sekli).toString());

        if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {
            if ( M_FIS->fis_turu EQ ENUM_IRS_NORMAL_CIKIS ) {
                comboBox_kdv_orani->setCurrentIndex(comboBox_kdv_orani->findText(query.VALUE(3).toString()));
                comboBox_otv_orani->setCurrentIndex(comboBox_otv_orani->findText(query.VALUE(4).toString()));
            }// cikis
            else {
                comboBox_kdv_orani->setCurrentIndex(comboBox_kdv_orani->findText(query.VALUE(5).toString()));
                comboBox_otv_orani->setCurrentIndex(comboBox_otv_orani->findText(query.VALUE(6).toString()));
            }
        }
        else {
            if ( M_FIS->fis_turu EQ ENUM_STK_STOK_ACILIS_FISI ) {
                comboBox_kdv_orani->setCurrentIndex(comboBox_kdv_orani->findText(query.VALUE(3).toString()));
                comboBox_otv_orani->setCurrentIndex(comboBox_otv_orani->findText(query.VALUE(4).toString()));
            }// ENUM_STK_SARF_FIRE_FISI
            else {
                comboBox_kdv_orani->setCurrentIndex(comboBox_kdv_orani->findText(query.VALUE(5).toString()));
                comboBox_otv_orani->setCurrentIndex(comboBox_otv_orani->findText(query.VALUE(6).toString()));
            }
        }


        commaEdit_satir_stok_miktari->setDisabled ( false );
        comboBox_stok_birimi->setDisabled         ( false );

        switch ( stok_takip_sekli ) {
            case ENUM_SERI_NO_ILE_TAKIP :
                commaEdit_satir_stok_miktari->setDisabled ( true );
                comboBox_stok_birimi->setDisabled         ( true );
                pushButton_seri_no_parti_kodu->setEnabled ( true );
                break;
            case ENUM_PARTI_TAKIBI      :
                pushButton_seri_no_parti_kodu->setEnabled ( true );
                break;
            case ENUM_BASIT_TAKIP :
            default          :
                pushButton_seri_no_parti_kodu->setEnabled (false);
                break;
        }


        STK_FILL_URUN_BIRIMLERI_COMBOBOX ( urun_id, comboBox_stok_birimi, M_STOK_BIRIM_ADLARI );
        URUN_BILGILERINI_AL              ( p_row_number );
        URUN_BIRIMI_FIYATLARINI_HESAPLA  ( p_row_number );

        double vergi_haric_tutar = DVZ_PARA_BIRIMLERINI_DONUSTUR ( M_FIS->stk_irs_para_birimi_id,
                                                             commaEdit_satir_tutari->GET_DOUBLE(),
                                                             m_temel_para_birimi_id ,
                                                             m_ui->adakDate_fis_tarihi->QDATE());

        commaEdit_vergi_haric_tutar->SET_DOUBLE ( ROUND ( vergi_haric_tutar ) );

        double otv_orani = comboBox_otv_orani->currentText().toDouble();
        double kdv_orani = comboBox_kdv_orani->currentText().toDouble();

        commaEdit_otv_tutari->SET_DOUBLE ( ROUND ( vergi_haric_tutar * ( otv_orani / 100 )  ) );

        double otv_tutari       = commaEdit_otv_tutari->GET_DOUBLE();

        commaEdit_otv_dahil_tutar->SET_DOUBLE ( ROUND ( vergi_haric_tutar + otv_tutari ) );

        double otv_dahil_tutar  = commaEdit_otv_dahil_tutar->GET_DOUBLE();

        commaEdit_kdv_tutari->SET_DOUBLE ( ROUND ( otv_dahil_tutar * ( kdv_orani / 100 ) ) );

        double kdv_tutari       = commaEdit_kdv_tutari->GET_DOUBLE();

        commaEdit_kdv_dahil_tutar->SET_DOUBLE ( ROUND ( otv_dahil_tutar + kdv_tutari ) );

        commaEdit_otv_dahil_tutar->SET_DOUBLE ( otv_dahil_tutar );

        TOPLU_ISKONTO_SONRASI_SATIR_DEGERLERINI_HESAPLA ( p_row_number );

        SET_FOCUS( commaEdit_satir_stok_miktari );
    }

    else if ( ( p_object EQ commaEdit_satir_tutari )            OR ( p_object EQ commaEdit_satir_stok_miktari )
            OR ( p_object EQ commaEdit_isk_oncesi_birim_fiyat )
            OR ( p_object EQ commaEdit_isk_oncesi_birim_fiyat ) OR ( p_object EQ commaEdit_iskonto_yuzdesi )
            OR ( p_object EQ comboBox_stok_birimi )             OR ( p_object EQ comboBox_kdv_orani )
            OR ( p_object EQ comboBox_kdv_dahil_mi )            OR ( p_object EQ comboBox_otv_orani ) ) {


        return IRSALIYE_SATIR_TUTARLARI_HESAPLA( p_row_number, p_object );

    }
    else if ( p_object EQ pushButton_seri_no_parti_kodu ) {

        int stok_takip_sekli = lineEdit_stok_takip_sekli->text().toInt();


        switch ( stok_takip_sekli ) {

            case ENUM_BASIT_TAKIP       :
            case ENUM_SERI_NO_ILE_TAKIP :
            default                :
                break;

            case ENUM_PARTI_TAKIBI :
            {

                int parti_id = STK_GET_PARTI_ID( lineEdit_seri_no_list->text() );
                if ( parti_id EQ 0 ) {
                    if ( M_FIS->irsaliye_alis_satis_turu EQ ENUM_SATIS_IRSALIYESI_FATURASI ) {
                        MSG_WARNING(  tr("Parti kodu bulunamadı."), pushButton_seri_no_parti_kodu );

                        return ADAK_FAIL_UNDO;
                    }
                    lineEdit_parti_id->clear();
                    break;
                }

                lineEdit_parti_id->setText(QVariant(parti_id).toString());
            }
                break;
        }

    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int STOK_FISI::CHECK_LINE_EMPTY ( int p_row_number )
{
    if ( M_FIS->depo_id < 1 ) {
        MSG_WARNING(  tr("Önce depo seçimi yapmalısınız."), m_ui->searchEdit_depo_kodu );

        return ADAK_FAIL;
    }

    switch ( M_FIS->fis_turu ) {
        case ENUM_STK_DEPO_TRANSFER_GIRIS   :
        case ENUM_STK_DEPO_TRANSFER_CIKIS   :
            if ( m_ui->searchEdit_alan_depo_kodu->GET_TEXT().isEmpty() EQ true ) {
                MSG_WARNING(  tr("Alan depo kodu boş bırakılamaz"), m_ui->searchEdit_alan_depo_kodu );

                return ADAK_FAIL;
            }
            if ( M_FIS->alan_depo_id < 1 ) {
                MSG_WARNING(  tr("Alan depo seçimi yapılmamış"), m_ui->searchEdit_alan_depo_kodu );

                return ADAK_FAIL;
            }
            break;

        default :
            break;
    }

    QLineEdit   * lineEdit_urun_adi             = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_AD_COLUMN             );
    QLineEdit   * lineEdit_urun_id              = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN              );
    QSearchEdit * searchEdit_urun_kodu          = ( QSearchEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_KOD_COLUMN            );
    QLineEdit   * lineEdit_stok_takip_sekli     = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_TAKIP_SEKLI_COLUMN     );
    QPushButton * pushButton_seri_no_parti_kodu = ( QPushButton * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SERI_NO_PARTI_KODU_COLUMN  );
    QLineEdit   * lineEdit_seri_no_list         = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SERI_NO_LISTE_COLUMN        );
    QLineEdit   * lineEdit_parti_id             = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, PARTI_ID_COLUMN             );
    QCommaEdit  * commaEdit_satir_stok_miktari  = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_MIKTARI_COLUMN         );
    QComboBox   * comboBox_urun_birimi          = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN          );


    switch ( M_FIS->fis_turu ) {
        case ENUM_IRS_NORMAL_GIRIS   :
        case ENUM_IRS_NORMAL_CIKIS   :
        case ENUM_IRS_KONSINYE_GIRIS :
        case ENUM_IRS_KONSINYE_CIKIS :
            if ( lineEdit_urun_adi->text().isEmpty() EQ true ) {
                MSG_WARNING(  tr("Ürün adı boş bırakılamaz."), lineEdit_urun_adi );

                return ADAK_FAIL;
            }
            break;

        case ENUM_IRS_FASON_BASLANGIC_GIRIS :
        case ENUM_IRS_FASON_BASLANGIC_CIKIS :
        case ENUM_IRS_FASON_BITIS_GIRIS     :
        case ENUM_IRS_FASON_BITIS_CIKIS     :
        case ENUM_STK_SARF_FIRE_FISI        :
        case ENUM_STK_IMALATA_SEVK_FISI     :
        case ENUM_STK_IMALATTAN_DONUS_FISI  :
        case ENUM_STK_STOK_ACILIS_FISI      :

        default :

            if ( lineEdit_urun_id->text().toInt() < 1 ) {
                MSG_WARNING(  tr("Ürün seçimi yapılmamış"), lineEdit_urun_id );

                return ADAK_FAIL;
            }
            if ( searchEdit_urun_kodu->GET_TEXT().isEmpty() EQ true )  {
                MSG_WARNING(  tr("Ürün kodu boş bırakılamaz"), searchEdit_urun_kodu );

                return ADAK_FAIL;
            }
            break;
    }


    int urun_id = lineEdit_urun_id->text().toInt();

    if ( urun_id < 1 ) {
        if ( commaEdit_satir_stok_miktari->GET_DOUBLE() > 0 ) {
            if ( comboBox_urun_birimi->currentText().isEmpty() EQ true ) {
                MSG_WARNING(  tr("Ürün miktarı girdiğiniz ürün için temel birim de girmelisiniz."), comboBox_urun_birimi );

                return ADAK_FAIL;
            }
        }                           // Eğer ürün kartı olmayan bir ürün girişi yapılmışsa ( normal irsaliye veya konsinye )
        return ADAK_OK;            // stok takip şekline göre herangi bir kontrol yapılmayacaktır.
    }

    int stok_takip_sekli = lineEdit_stok_takip_sekli->text().toInt();


    int irsaliye_stok_fisi_alis_satis_turu = IRSALIYE_ALIS_SATIS_TURUNU_BELIRLE ();

    switch ( irsaliye_stok_fisi_alis_satis_turu ) {

        case ENUM_ALIS_IRSALIYESI_FATURASI :
        default                            : {
            //! HATALI SATIR OLUSMAMASI ICIN KONTROL
            if ( commaEdit_satir_stok_miktari->GET_DOUBLE() EQ 0.00 ) {
                MSG_WARNING( "Ürün miktarı boş geçilemez.!", NULL );
                if ( stok_takip_sekli EQ ENUM_BASIT_TAKIP ) {
                    SET_FOCUS( commaEdit_satir_stok_miktari );
                }
                else {
                    SET_FOCUS( pushButton_seri_no_parti_kodu  );
                }
                return ADAK_FAIL;
            }

            switch ( stok_takip_sekli ) {

                case ENUM_BASIT_TAKIP       :
                case ENUM_SERI_NO_ILE_TAKIP :
                default                :
                    break;

                case ENUM_PARTI_TAKIBI :
                    if ( lineEdit_seri_no_list->text().isEmpty() EQ true ) {
                        MSG_WARNING(  tr("Bu stok, parti takip yöntemi ile takip ediliyor.Parti kodu alanı boş bırakılamaz."), pushButton_seri_no_parti_kodu );

                        return ADAK_FAIL;
                    }
                    break;
            }
            break;
        }
        case ENUM_SATIS_IRSALIYESI_FATURASI :

            switch ( stok_takip_sekli ) {

                case ENUM_BASIT_TAKIP       :
                case ENUM_SERI_NO_ILE_TAKIP :
                default                :
                    break;

                case ENUM_PARTI_TAKIBI :
                    if ( lineEdit_seri_no_list->text().isEmpty() EQ true ) {
                        MSG_WARNING(  tr("Bu stok, parti takip yöntemi ile takip ediliyor.Parti kodu alanı boş bırakılamaz."), pushButton_seri_no_parti_kodu );

                        return ADAK_FAIL;
                    }
                    if ( lineEdit_parti_id->text().toInt() EQ 0 ) {
                        MSG_WARNING(  tr("Bu stok, parti takip yöntemi ile takip ediliyor. Depodan uygun bir parti seçiniz"), pushButton_seri_no_parti_kodu );

                    }
                    break;
            }

            break;
    }


    return ADAK_OK;
}


/**************************************************************************************
                   STOK_FISI::CHECK_ADD_LINE
***************************************************************************************/

int STOK_FISI::CHECK_ADD_LINE ( int p_fis_id, int p_row_number )
{

    switch( M_FIS->fis_turu ) {
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS:
            break;

        default:
            if ( STOK_FISINDE_DEGISIKLIK_YAPILABILIR_MI( p_fis_id ) EQ false ) {
                return ADAK_FAIL;
            }
    }

    SQL_QUERY query ( DB );

    QLineEdit   * lineEdit_urun_id                      = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN                  );
    QCommaEdit  * commaEdit_stok_miktari                = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_MIKTARI_COLUMN             );
    QCommaEdit  * commaEdit_temel_birim_katsayisi       = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, TEMEL_BIRIM_KATSAYISI           );
    QLineEdit   * lineEdit_stok_takip_sekli             = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_TAKIP_SEKLI_COLUMN         );
    QLineEdit   * lineEdit_parti_id                     = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, PARTI_ID_COLUMN                 );
    QLineEdit   * lineEdit_fatura_satiri_id             = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, FATURA_SATIRI_ID_COLUMN         );
    QLineEdit   * lineEdit_iade_edilen_irs_satir_id     = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, IADE_EDILEN_IRS_SATIR_ID_COLUMN );
    QComboBox   * combo_box_urun_birimi                 = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN );
    QLineEdit   * lineEdit_seri_no_list                 = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SERI_NO_LISTE_COLUMN             );
    QPushButton * pushButton_seri_no                    = ( QPushButton * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SERI_NO_PARTI_KODU_COLUMN        );

    int urun_id                         = lineEdit_urun_id->text().toInt();
    int fatura_satiri_id                = lineEdit_fatura_satiri_id->text().toInt();
    int iade_edilen_irs_satir_id        = lineEdit_iade_edilen_irs_satir_id->text().toInt();


    if ( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
        if ( iade_edilen_irs_satir_id < 1 ) {
            MSG_WARNING(  tr("İade işleminde satırlar sadece iade satırı seç butonu ile girilebilir, elle satır giremesiniz."), NULL );
            return ADAK_FAIL;
        }
    }

    switch ( M_FIS->fis_turu ) {

        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS  :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS :
        {
            FATURA_FIS_SATIRI_STRUCT FAT_FIS_SATIRI;
            FATURA_CLEAR_FIS_SATIRI_STRUCT ( &FAT_FIS_SATIRI );

            FATURA_SATIR_BILGILERINI_OKU( &FAT_FIS_SATIRI, fatura_satiri_id );

            double  satirdaki_urun_miktari  =   FAT_FIS_SATIRI.satirdaki_urun_miktari;
            double  irsaliyelenen_miktar    =   FAT_FIS_SATIRI.irsaliyelenen_miktar;
            double  kalan_miktar            =   satirdaki_urun_miktari - irsaliyelenen_miktar;

            if ( commaEdit_stok_miktari->GET_DOUBLE() > kalan_miktar ) {
                MSG_WARNING(  tr("Seçili fatura satırı için irsaliyenenmemiş ürün miktari ") + QVariant( kalan_miktar ).toString().toUtf8() + tr(". Bu miktarı aşamazsınız."), commaEdit_stok_miktari );

                return ADAK_FAIL;
            }

            if (lineEdit_stok_takip_sekli->text().toInt() EQ ENUM_SERI_NO_ILE_TAKIP ) {
                QStringList seri_no_list = lineEdit_seri_no_list->text().split(",");
                seri_no_list.removeAll("");
                seri_no_list.removeAll(" ");
                if ( commaEdit_stok_miktari->GET_DOUBLE() NE seri_no_list.size() ) {
                    MSG_WARNING("Lütfen Seri Noları girin", pushButton_seri_no );
                    return ADAK_FAIL;
                }
            }

            break;
        }


        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS  :
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS :

            MSG_WARNING(  tr("Otomatik fatura irsaliyeleştirme yaparken, elle satır giremezsiniz."), NULL );
            return ADAK_FAIL;

        default :
            break;
    }

    if ( urun_id < 1 ) {
        return ADAK_OK;    // Buraya kadar ürün seçimi olmadan gelindiyse konsinye
    }                        //veya normal irsaliyeden ürün kartı olmayan ürün girilmiş demektir.


    query.PREPARE_SELECT("stk_fisler","fis_tarihi, depo_id, alan_depo_id ",
                         "fis_id = :fis_id");
    query.SET_VALUE(":fis_id", p_fis_id );
    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr("Fiş bilgileri bulunamadı."), NULL );
        return ADAK_FAIL;
    }
    query.NEXT();

    int depo_id      = 0;

    switch ( M_FIS->fis_turu ) {

        case ENUM_STK_DEPO_TRANSFER_CIKIS :
        default                      :
            depo_id = query.VALUE(1).toInt();
            break;

        case ENUM_STK_DEPO_TRANSFER_GIRIS :
            depo_id = query.VALUE(2).toInt();
            break;
    }

    int   stok_takip_sekli  = lineEdit_stok_takip_sekli->text().toInt();

    int     parti_id               = lineEdit_parti_id->text().toInt();
    double  stok_miktari           = commaEdit_stok_miktari->GET_DOUBLE() * commaEdit_temel_birim_katsayisi->GET_DOUBLE();

    /*
     * Satistan iadede depo kontrolu yapmaya gerek yok fakat.
     * alis irsaliyesi gibi davrandigi icin
     * sadece satirdan fazla miktarda girilmesi engellenmeli
     */

    if( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
        if( M_FIS->irsaliye_alis_satis_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI ) {
            // satir miktarindan fazla iade edemez
            SQL_QUERY query( DB );

            query.PREPARE_SELECT( "stk_fis_satirlari", "satirdaki_urun_miktari, iade_edilen_miktar", "fis_satiri_id = :fis_satiri_id"  );
            query.SET_VALUE     ( ":fis_satiri_id", iade_edilen_irs_satir_id );

            if( query.SELECT() NE 0 ) {
                query.NEXT();
                double satirdaki_urun_miktari = query.VALUE( "satirdaki_urun_miktari").toDouble();
                double iade_edilen_miktar     = query.VALUE( "iade_edilen_miktar").toDouble();

                double kalan_miktar = satirdaki_urun_miktari - iade_edilen_miktar;

                if( stok_miktari > kalan_miktar ) {

                    MSG_WARNING( "Satır miktarından fazla ürün iade edemessiniz", commaEdit_stok_miktari );
                    return ADAK_FAIL;
                }
            }
        }
    }

    // alis iadede depo kontrolu yapiliyor.
    if ( STK_IRSALIYEYE_SATIR_EKLENMESI_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU ( m_ui->adakDate_fis_tarihi->QDATE(), M_FIS->irsaliye_alis_satis_turu, stok_takip_sekli, urun_id, depo_id, parti_id, stok_miktari, combo_box_urun_birimi->currentText(), this ) EQ false ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_FISI::ADD_LINE
***************************************************************************************/

void STOK_FISI::ADD_LINE ( int p_fis_id, int p_row_number )
{
    QLineEdit   * lineEdit_urun_id                          = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN                      );
    QLineEdit   * lineEdit_urun_adi                         = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_AD_COLUMN                      );
    QCommaEdit  * commaEdit_urun_miktari                    = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_MIKTARI_COLUMN                 );
    QComboBox   * comboBox_urun_birimi                      = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN                  );
    QCommaEdit  * commaEdit_temel_birim_katsayisi           = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, TEMEL_BIRIM_KATSAYISI               );
    QCommaEdit  * commaEdit_isk_oncesi_birim_fiyat          = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN       );
    QCommaEdit  * commaEdit_isk_sonrasi_birim_fiyat         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN      );
    QCommaEdit  * commaEdit_iskonto_yuzdesi                 = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISKONTO_YUZDESI_COLUMN              );
    QCommaEdit  * commaEdit_iskonto_tutari                  = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISKONTO_TUTARI_COLUMN               );
    QCommaEdit  * commaEdit_satir_tutari                    = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN                 );
    QLineEdit   * lineEdit_para_birimi                      = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, PARA_BIRIMI_COLUMN                  );
    QCommaEdit  * commaEdit_vergi_haric_tutar               = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTARI_COLUMN           );
    QLineEdit   * lineEdit_stok_takip_sekli                 = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_TAKIP_SEKLI_COLUMN             );
    QLineEdit   * lineEdit_parti_id                         = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, PARTI_ID_COLUMN                     );
    QComboBox   * comboBox_kdv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_ORANI_COLUMN                    );
    QCommaEdit  * commaEdit_kdv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_TUTAR_COLUMN                    );
    QCommaEdit  * commaEdit_kdv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_DAHIL_TUTARI_COLUMN             );
    QComboBox   * comboBox_kdv_dahil_mi                     = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN                 );
    QCommaEdit  * commaEdit_otv_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_TUTARI_COLUMN                   );
    QCommaEdit  * commaEdit_otv_dahil_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_DAHIL_TUTAR_COLUMN              );
    QComboBox   * comboBox_otv_orani                        = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_ORANI_COLUMN                    );
    QLineEdit   * lineEdit_fatura_satiri_id                 = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, FATURA_SATIRI_ID_COLUMN             );
    QLineEdit   * lineEdit_iade_edilen_irs_satir_id         = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, IADE_EDILEN_IRS_SATIR_ID_COLUMN     );
    QLineEdit   * lineEdit_hatali_satir_mi                  = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, HATALI_SATIR_MI_COLUMN              );
    QCommaEdit  * commaEdit_toplu_iskonto_yuzdesi           = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, TOPLU_ISKONTO_YUZDESI_COLUMN        );
    QCommaEdit  * commaEdit_t_is_sonrasi_birim_fiyat        = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_BIRIM_FIYAT_COLUMN     );
    QCommaEdit  * commaEdit_t_is_sonrasi_satir_tutari       = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_SATIR_TUTARI_COLUMN    );
    QCommaEdit  * commaEdit_t_is_sonrasi_vergi_haric_tutar  = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN );
    QCommaEdit  * commaEdit_t_is_sonrasi_otv_tutari         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_OTV_TUTARI_COLUMN        );
    QCommaEdit  * commaEdit_t_is_sonrasi_otv_dahil_tutar    = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN   );
    QCommaEdit  * commaEdit_t_is_sonrasi_kdv_tutari         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_KDV_TUTARI_COLUMN        );
    QCommaEdit  * commaEdit_t_is_sonrasi_kdv_dahil_tutar    = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN   );
    QLineEdit   * lineEdit_seri_no_list                     = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SERI_NO_LISTE_COLUMN                  );


    int modul_id = -1;
    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_STOK_FISI ) {
        modul_id = STOK_MODULU;
    }
    else if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {
        modul_id = IRSALIYE_MODULU;
    }

    STK_IRS_CLEAR_SATIR_STRUCT( M_FIS_SATIRI );

    M_FIS_SATIRI->urun_id                        = lineEdit_urun_id->text().toInt();
    M_FIS_SATIRI->order_number                   = m_ui->tableWidget_fis_satirlari->item ( p_row_number, ORDER_NUM_COLUMN )->text().toInt();
    M_FIS_SATIRI->fis_id                         = p_fis_id;
    M_FIS_SATIRI->urun_adi                       = lineEdit_urun_adi->text();
    M_FIS_SATIRI->satirdaki_urun_miktari         = commaEdit_urun_miktari->GET_DOUBLE();
    M_FIS_SATIRI->iade_edilen_miktar             = commaEdit_urun_miktari->GET_DOUBLE();
    M_FIS_SATIRI->urun_birimi                    = comboBox_urun_birimi->currentText();
    M_FIS_SATIRI->temel_birim_katsayisi          = commaEdit_temel_birim_katsayisi->GET_DOUBLE();
    M_FIS_SATIRI->isk_oncesi_urun_birim_fiyati   = commaEdit_isk_oncesi_birim_fiyat->GET_DOUBLE();
    M_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati  = commaEdit_isk_sonrasi_birim_fiyat->GET_DOUBLE();
    M_FIS_SATIRI->iskonto_yuzdesi                = commaEdit_iskonto_yuzdesi->GET_DOUBLE();
    M_FIS_SATIRI->iskonto_tutari                 = commaEdit_iskonto_tutari->GET_DOUBLE();
    M_FIS_SATIRI->satir_tutari                   = commaEdit_satir_tutari->GET_DOUBLE();
    M_FIS_SATIRI->doviz_id                       = DVZ_GET_DOVIZ_ID( lineEdit_para_birimi->text() );
    M_FIS_SATIRI->doviz_kodu                     = lineEdit_para_birimi->text();
    M_FIS_SATIRI->kdv_orani                      = comboBox_kdv_orani->currentText().toDouble();
    M_FIS_SATIRI->kdv_dahil_mi                   = comboBox_kdv_dahil_mi->currentIndex();
    M_FIS_SATIRI->satir_vergi_haric_tutar        = commaEdit_vergi_haric_tutar->GET_DOUBLE();
    M_FIS_SATIRI->satir_kdv_tutari               = commaEdit_kdv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->satir_kdv_dahil_tutar          = commaEdit_kdv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->otv_orani                      = comboBox_otv_orani->currentText().toDouble();
    M_FIS_SATIRI->satir_otv_tutari               = commaEdit_otv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->satir_otv_dahil_tutar          = commaEdit_otv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->parti_id                       = lineEdit_parti_id->text().toInt();
    M_FIS_SATIRI->parti_kodu                     = lineEdit_seri_no_list->text();
    M_FIS_SATIRI->stok_takip_sekli               = lineEdit_stok_takip_sekli->text().toInt();
    M_FIS_SATIRI->fatura_satiri_id               = lineEdit_fatura_satiri_id->text().toInt();
    M_FIS_SATIRI->iade_edilen_irsaliye_satir_id  = lineEdit_iade_edilen_irs_satir_id->text().toInt();
    M_FIS_SATIRI->row_number                     = p_row_number;
    M_FIS_SATIRI->hatali_satir_mi                = 0;
    M_FIS_SATIRI->toplu_iskonto_yuzdesi          = commaEdit_toplu_iskonto_yuzdesi->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_birim_fiyat       = commaEdit_t_is_sonrasi_birim_fiyat->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_satir_tutari      = commaEdit_t_is_sonrasi_satir_tutari->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_vergi_haric_tutar = commaEdit_t_is_sonrasi_vergi_haric_tutar->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_otv_tutari        = commaEdit_t_is_sonrasi_otv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_otv_dahil_tutar   = commaEdit_t_is_sonrasi_otv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_kdv_tutari        = commaEdit_t_is_sonrasi_kdv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_kdv_dahil_tutar   = commaEdit_t_is_sonrasi_kdv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->modul_id                       = modul_id;
    M_FIS_SATIRI->program_id                     = E9_PROGRAMI;

    if( lineEdit_seri_no_list->text().isEmpty() EQ false ) {
        M_FIS_SATIRI->seri_no_list  = lineEdit_seri_no_list->text().split(",");
    }

    if ( lineEdit_urun_id->text().toInt() > 0 ) {
        if ( commaEdit_urun_miktari->GET_DOUBLE() EQ 0 ) {
            M_FIS_SATIRI->hatali_satir_mi = 1;
        }
    }

    M_FIS_SATIRI->faturalanan_miktar = 0;

    if( M_FIS->fis_turu EQ ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS OR M_FIS->fis_turu EQ ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS ) {
        M_FIS_SATIRI->faturalanan_miktar = M_FIS_SATIRI->satirdaki_urun_miktari;
    }

    STOK_FISINE_YENI_SATIR_KAYDET ( M_FIS_SATIRI );

    switch ( M_FIS->fis_turu ) {
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS:
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS:
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS:
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS:
            FAT_UPDATE_FATURA_IRSALIYELESTIRILDI_MI( FAT_GET_FATURA_ID( M_FIS_SATIRI->fatura_satiri_id ), 1 );
            IRS_BAGLANTI_BILGISINI_GOSTER( M_FIS->fis_turu, p_fis_id );
            break;
        default:
            break;
    }

    lineEdit_parti_id->setText(QVariant(M_FIS_SATIRI->parti_id).toString());
    lineEdit_hatali_satir_mi->setText( QVariant ( M_FIS_SATIRI->hatali_satir_mi ).toString() );

}

/**************************************************************************************
                   STOK_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int STOK_FISI::CHECK_UPDATE_LINE ( int p_fis_id, int p_row_number )
{

    switch( M_FIS->fis_turu ) {
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS:
            break;

        default:
            if ( STOK_FISINDE_DEGISIKLIK_YAPILABILIR_MI( p_fis_id ) EQ false ) {
                return ADAK_FAIL;
            }
    }

    SQL_QUERY query ( DB );

    QLineEdit   * lineEdit_urun_id                      = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN             );
    QCommaEdit  * commaEdit_stok_miktari                = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_MIKTARI_COLUMN        );
    QCommaEdit  * commaEdit_temel_birim_katsayisi       = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, TEMEL_BIRIM_KATSAYISI      );
    QLineEdit   * lineEdit_stok_takip_sekli             = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_TAKIP_SEKLI_COLUMN    );
    QLineEdit   * lineEdit_parti_id                     = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, PARTI_ID_COLUMN            );
    QLineEdit   * lineEdit_fatura_satiri_id             = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, FATURA_SATIRI_ID_COLUMN    );
    QLineEdit   * lineEdit_iade_edilen_irs_satir_id     = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, IADE_EDILEN_IRS_SATIR_ID_COLUMN  );

    int fis_satiri_id          = m_ui->tableWidget_fis_satirlari->item ( p_row_number, ROW_ID_COLUMN )->text().toInt();
    int new_fatura_satiri_id   = lineEdit_fatura_satiri_id->text().toInt();


    query.PREPARE_SELECT("stk_fis_satirlari, stk_fisler",
                         "satirdaki_urun_miktari, parti_id, urun_id, "
                         "depo_id, fis_tarihi, temel_birim_katsayisi ",
                         "stk_fisler.fis_id = stk_fis_satirlari.fis_id "
                         "AND fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE(":fis_satiri_id", fis_satiri_id );
    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr("Satırdaki stok miktarı alınamadı"), NULL );
        return ADAK_FAIL;
    }
    query.NEXT();


    int     old_parti_id               = query.VALUE(1).toInt();
    int     old_urun_id                = query.VALUE(2).toInt();
    int     depo_id                    = query.VALUE(3).toInt();
    double  old_stok_miktari           = query.VALUE(0).toDouble() * query.VALUE(5).toDouble();;
    QDate   fis_tarihi                 = QDate::fromString ( query.VALUE(4).toString(), "yyyy.MM.dd");

    int     new_urun_id               = lineEdit_urun_id->text().toInt();
    int     new_stok_takip_sekli      = lineEdit_stok_takip_sekli->text().toInt();
    double  new_stok_miktari          = commaEdit_stok_miktari->GET_DOUBLE() * commaEdit_temel_birim_katsayisi->GET_DOUBLE();
    //QString new_seri_no_parti_kodu    = searchEdit_seri_no_parti_kodu->GET_TEXT();
    int     new_parti_id              = lineEdit_parti_id->text().toInt();
    int     gun_no                    = MALI_YIL_ARRAY_INDIS ( fis_tarihi );

    if ( M_FIS->fis_turu EQ ENUM_STK_STOK_ACILIS_FISI ) {
        gun_no = 0;
    }

    switch ( M_FIS->fis_turu ) {

        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS  :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS :
        {
            query.PREPARE_SELECT("fat_irs_satir_baglantisi",
                                 "fatura_satiri_id",
                                 "irsaliye_satiri_id = :irsaliye_satiri_id");
            query.SET_VALUE(":irsaliye_satiri_id", fis_satiri_id );
            if ( query.SELECT() EQ 0 ) {
                break;
            }
            query.NEXT();

            int old_fatura_satiri_id = query.VALUE(0).toInt();

            FATURA_FIS_SATIRI_STRUCT FAT_FIS_SATIRI;

            FATURA_CLEAR_FIS_SATIRI_STRUCT ( &FAT_FIS_SATIRI );

            int result = FATURA_SATIR_BILGILERINI_OKU( &FAT_FIS_SATIRI, old_fatura_satiri_id );

            if ( result EQ 0 ) {
                break;
            }

            double  old_satirdaki_urun_miktari  =   FAT_FIS_SATIRI.satirdaki_urun_miktari;
            double  old_irsaliyelenen_miktar    =   FAT_FIS_SATIRI.irsaliyelenen_miktar;
            double  old_kalan_miktar            =   old_satirdaki_urun_miktari - old_irsaliyelenen_miktar;

            if ( old_fatura_satiri_id EQ new_fatura_satiri_id ) {
                if ( new_stok_miktari > old_kalan_miktar + old_stok_miktari ) {
                    MSG_WARNING(  tr("Seçili fatura satırı için irsaliyenenmemiş ürün miktari ") + QVariant( old_kalan_miktar ).toString().toUtf8() + tr(". Bu miktarı aşamazsınız."), NULL );
                    return ADAK_FAIL;
                }
            }
            else {
                FATURA_FIS_SATIRI_STRUCT FAT_FIS_SATIRI;

                FATURA_CLEAR_FIS_SATIRI_STRUCT ( &FAT_FIS_SATIRI );

                int result = FATURA_SATIR_BILGILERINI_OKU( &FAT_FIS_SATIRI, old_fatura_satiri_id );
                if ( result EQ 0 ) {
                    break;
                }
                query.NEXT();
                double  new_satirdaki_urun_miktari  = FAT_FIS_SATIRI.satirdaki_urun_miktari;
                double  new_irsaliyelenen_miktar    = FAT_FIS_SATIRI.irsaliyelenen_miktar;
                double  new_kalan_miktar            = new_satirdaki_urun_miktari - new_irsaliyelenen_miktar;

                if ( new_stok_miktari > new_kalan_miktar ) {
                    if ( new_stok_miktari > old_kalan_miktar + old_stok_miktari ) {
                        MSG_WARNING(  tr("Seçili fatura satırı için irsaliyenenmemiş ürün miktari ") + QVariant( new_kalan_miktar ).toString().toUtf8() + tr(". Bu miktarı aşamazsınız."), NULL );
                        return ADAK_FAIL_UNDO;
                    }
                }
            }
            break;
        }
        default :
            break;
    }

    int irsaliye_stok_fisi_alis_satis_turu = IRSALIYE_ALIS_SATIS_TURUNU_BELIRLE ();


    if ( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
        query.PREPARE_SELECT("irs_iade_satir_baglantisi","irs_iade_edilen_satir_id",
                             "irs_satir_id = :irs_satir_id");
        query.SET_VALUE(":irs_satir_id", fis_satiri_id );
        if ( query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr("İrsaliye satır bilgileri okunamadı"), NULL );
            return ADAK_FAIL;
        }
        query.NEXT();

        if ( query.VALUE(0).toInt() EQ lineEdit_iade_edilen_irs_satir_id->text().toInt() ) {
            // idl ler ayni ise varolan satirin miktari ile oynaniyordur
            if ( IRS_IADE_IRSALIYE_SATIR_GUNCELLE_ICIN_MIKTAR_KONTROLU ( lineEdit_iade_edilen_irs_satir_id->text().toInt(), commaEdit_stok_miktari->GET_DOUBLE(), this ) EQ false ) {
                return ADAK_FAIL;
            }
        }
        else {
            // id ler ayni degilse baska satir secilmistir yeni bir satir icin miktar kontrolu yapilir
            if ( IRS_IADE_IRSALIYE_SATIR_EKLE_ICIN_MIKTAR_KONTROLU ( lineEdit_iade_edilen_irs_satir_id->text().toInt(), commaEdit_stok_miktari->GET_DOUBLE(), this ) EQ false ) {
                return ADAK_FAIL;
            }
        }
    }
    else {

        if ( STK_IRSALIYE_SATIR_GUNCELLEME_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU ( irsaliye_stok_fisi_alis_satis_turu,
                                                                                  new_urun_id,
                                                                                  new_stok_takip_sekli,
                                                                                  depo_id,
                                                                                  new_stok_miktari,
                                                                                  new_parti_id,
                                                                                  old_urun_id,
                                                                                  old_stok_miktari,
                                                                                  old_parti_id,
                                                                                  gun_no,
                                                                                  this ) EQ false ) {
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_FISI::UPDATE_LINE
***************************************************************************************/

void STOK_FISI::UPDATE_LINE ( int p_fis_id, int p_row_number )
{
    STOK_FIS_SATIR_BILGILERINI_GUNCELLE ( p_fis_id, p_row_number );
    SELECTED_LINE(p_row_number, -1);
}

/**************************************************************************************
                   STOK_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int STOK_FISI::CHECK_DELETE_LINE ( int p_fis_id, int p_row_number )
{
    switch( M_FIS->fis_turu ) {
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS:
            break;

        default:
            if ( STOK_FISINDE_DEGISIKLIK_YAPILABILIR_MI( p_fis_id ) EQ false ) {
                return ADAK_FAIL;
            }
    }

    switch ( M_FIS->fis_turu ) {

        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS  :
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS :

            MSG_WARNING(  tr("Otomatik fatura irsaliyeleştırme yaparken, elle satır silemezsiniz."), NULL );
            return ADAK_FAIL;

        default :
            break;
    }

    int fis_satiri_id = m_ui->tableWidget_fis_satirlari->item( p_row_number, ROW_ID_COLUMN )->text().toInt();

    if ( IADE_ISLEMI_YAPILMIS_SATIR_MI ( fis_satiri_id ) EQ false ) {
        return ADAK_FAIL;
    }

    int irsaliye_stok_fisi_alis_satis_turu = IRSALIYE_ALIS_SATIS_TURUNU_BELIRLE ();

    if ( STK_IRSALIYE_SATIR_SILME_DURUMU_ICIN_STOK_MIKTARLARI_KONTROLU ( fis_satiri_id, irsaliye_stok_fisi_alis_satis_turu, this ) EQ false ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_FISI::DELETE_LINE
***************************************************************************************/

void STOK_FISI::DELETE_LINE ( int p_fis_id, int p_row_number )
{
    int fis_satiri_id = m_ui->tableWidget_fis_satirlari->item( p_row_number, ROW_ID_COLUMN )->text().toInt();

    STOK_FIS_SATIRINI_SIL ( p_fis_id, fis_satiri_id );

}

/**************************************************************************************
                   STOK_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int STOK_FISI::LOCK_FIS_RECORD ( int p_fis_id )
{
    return DB->LOCK_ROW( "fis_id", "stk_fisler", QString ( "fis_id = %1" ).arg ( p_fis_id ) );
}

/**************************************************************************************
                   STOK_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void STOK_FISI::UNLOCK_FIS_RECORD ( int p_fis_id )
{
    DB->UNLOCK_ROW( "fis_id","stk_fisler", QString ( "fis_id = %1" ).arg ( p_fis_id) );
}

/**************************************************************************************
                   STOK_FISI::FIS_EKRANINI_DUZENLE
***************************************************************************************/

void STOK_FISI::FIS_EKRANINI_DUZENLE ()
{

    m_ui->commaEdit_bors_alacak_durumu->setVisible( false );
    m_ui->label_cari_borc_alacak->setVisible( false );
    m_ui->lineEdit_cari_para_birimi->setVisible( false );

    m_ui->commaEdit_cari_para_birimi_tutari->setVisible( false );
    m_ui->label_cari_para_birimi_tutari->setVisible( false );

    m_ui->label_bilgilendirme->setHidden(true);

    E9_KULLANICI_YETKILERI_STRUCT * KULLANICI_YETKILERI = ( E9_KULLANICI_YETKILERI_STRUCT * ) E9_GET_KULLANICI_YETKILERI();

    if ( KULLANICI_YETKILERI->irs_toplu_iskonto_yapabilir_mi EQ 0 ) {
        m_ui->label_iskonto_yuzdesi->setVisible     ( false );
        m_ui->label_iskonto->setVisible             ( false );
        m_ui->commaEdit_iskonto_tutari->setVisible  ( false );
        m_ui->commaEdit_iskonto_yuzdesi->setVisible ( false );
    }

    switch ( M_FIS->fis_turu ) {

        case ENUM_STK_IMALATTAN_DONUS_FISI               :
        case ENUM_STK_STOK_ACILIS_FISI                   :
        case ENUM_IRS_NORMAL_GIRIS                       :
        case ENUM_STK_DEPO_TRANSFER_GIRIS                :
        case ENUM_IRS_FASON_BASLANGIC_GIRIS              :
        case ENUM_IRS_FASON_BITIS_GIRIS                  :
        case ENUM_IRS_KONSINYE_GIRIS                     :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS  :
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS   :
        case ENUM_IRS_HAKEDIS_ALIS  :
        default                                          :
            M_FIS->irsaliye_alis_satis_turu = ENUM_ALIS_IRSALIYESI_FATURASI ;
            break;

        case ENUM_STK_SARF_FIRE_FISI                     :
        case ENUM_STK_IMALATA_SEVK_FISI                  :
        case ENUM_IRS_NORMAL_CIKIS                       :
        case ENUM_STK_DEPO_TRANSFER_CIKIS                :
        case ENUM_IRS_FASON_BASLANGIC_CIKIS              :
        case ENUM_IRS_FASON_BITIS_CIKIS                  :
        case ENUM_IRS_KONSINYE_CIKIS                     :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS :
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS  :
        case ENUM_IRS_HAKEDIS_SATIS:
            M_FIS->irsaliye_alis_satis_turu = ENUM_SATIS_IRSALIYESI_FATURASI;
            break;
    }

    m_ui->label_iptal->setVisible( false );

    switch ( M_FIS->irsaliye_mi_stok_fisi_mi ) {

        case ENUM_STOK_FISI :
        default             :
            STK_FILL_STOK_FIS_TURU_COMBOBOX ( m_ui->comboBox_irsaliye_turu );
            m_ui->label_irsaliye_turu->setText ( tr("Fiş Türü"));
            m_ui->comboBox_irsaliye_turu->setCurrentIndex ( m_ui->comboBox_irsaliye_turu->findText ( STK_GET_STOK_FIS_TURU ( M_FIS->fis_turu ) ) );
            if ( M_FIS->fis_turu EQ ENUM_STK_STOK_ACILIS_FISI ) {
               m_ui->comboBox_irsaliye_turu->clear();
               m_ui->comboBox_irsaliye_turu->addItem ( tr("STOK AÇILIŞ FİŞİ") );
               m_ui->comboBox_irsaliye_turu->setCurrentIndex(0);
               m_ui->comboBox_irsaliye_turu->setEnabled ( false );
            }

            break;

        case ENUM_IRSALIYE :
            IRS_FILL_IRSALIYE_TURU_COMBOBOX ( m_ui->comboBox_irsaliye_turu, M_FIS->irsaliye_alis_satis_turu, M_FIS->fis_turu );
            m_ui->comboBox_irsaliye_turu->setCurrentIndex ( m_ui->comboBox_irsaliye_turu->findText ( STK_GET_STOK_FIS_TURU ( M_FIS->fis_turu ) ) );
            m_ui->label_irsaliye_turu->setText ( tr ("İrsaliye Türü") );

            break;
    }

    m_ui->label_iade_edilecek_fis_no->setVisible( false );
    m_ui->commaEdit_iade_edilecek_fis_no->setVisible( false );
    m_ui->button_iade_edilecek_irsaliye_secimi->setVisible( false );

    if ( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
        m_ui->adakDate_fis_tarihi->setVisible( true );
        m_ui->label_iade_edilecek_fis_no->setVisible( true );
        m_ui->commaEdit_iade_edilecek_fis_no->setVisible( true );
        m_ui->button_iade_edilecek_irsaliye_secimi->setVisible( true );
    }

    QString isk_oncesi_birim_fiyat_header = tr ( "Birim Fiyat" );

    if ( KULLANICI_YETKILERI->irs_satir_iskontosu_yapabilir_mi EQ 1 ) {
        isk_oncesi_birim_fiyat_header.prepend(tr ( "İsk. Öncesi \n" ) );
    }


    m_ui->tableWidget_fis_satirlari->setHorizontalHeaderLabels(QStringList()<<"row_id"<<"fis_id"<<"order_COLUMN"
                                                             <<tr("stk_urun_id")<<tr("stok_takip_sekli")
                                                             <<tr("seri_no_parti_kodu_id")<<tr("fat_satiri_id")
                                                             <<tr("iade_irsaliye_satiri_id")
                                                             <<tr("hatali_satir_mi")<<tr("temel_birim_katsayisi")
                                                             <<tr("seri_no_list")
                                                             <<tr( "Fat. \nStr. Seç" ) << tr ( "İrs. \nStr. Seç" )
                                                             <<tr("Ürün \n Kodu/Barkod")<<tr("Ürün Adı")
                                                             <<tr("Seri No \nParti Kodu") <<tr("Miktarı")
                                                             <<tr("Birimi") <<isk_oncesi_birim_fiyat_header
                                                             <<tr("İskonto %")<<tr("İsk. Tutarı")
                                                             <<tr("İsk. Sonrası \n Birim Fiyat")
                                                             <<tr("toplu_is_yuzdesi")
                                                             <<tr("toplu_is_sonrasi_birim_fiyat")
                                                             <<tr("toplu_is_sonrasi_satir_tutari")
                                                             <<tr("toplu_is_sonrasi_vergi_haric_tutar")
                                                             <<tr("toplu_is_sonrasi_otv_tutari")
                                                             <<tr("toplu_is_sonrasi_otv_dahil_tutar")
                                                             <<tr("toplu_is_sonrasi_kdv_tutari")
                                                             <<tr("toplu_is_sonrasi_kdv_dahil_tutar")
                                                             <<tr("KDV")
                                                             <<tr("KDV %")<<tr("ÖTV %")<<tr("Fiyatı")
                                                             << m_temel_para_birimi_kodu + tr(" Tutarı")
                                                             <<tr("Para\nBirimi")
                                                             <<tr("kdv_tutari")<<tr("kdv_dahil_tutar")
                                                             <<tr("otv_tutari")<<tr("otv_dahil_tutar")
                                                             <<tr("")<<tr("") );

    m_ui->label_cari_hesap->setVisible           ( true );
    m_ui->lineEdit_cari_hesap_ismi->setVisible   ( true );
    m_ui->searchEdit_cari_hesap_kodu->setVisible ( true );
    m_ui->frame_adres_bilgileri->setVisible      ( true );

    m_ui->lineEdit_vergi_no->setVisible          ( true );
    m_ui->comboBox_vergi_dairesi->setVisible     ( true );
    m_ui->label_vergi_dairesi->setVisible        ( true );

    m_ui->tableWidget_fis_satirlari->hideColumn  ( TEMEL_BIRIM_KATSAYISI                  );
    m_ui->tableWidget_fis_satirlari->hideColumn  ( SERI_NO_LISTE_COLUMN                    );
    m_ui->tableWidget_fis_satirlari->hideColumn  ( KDV_TUTAR_COLUMN                       );
    m_ui->tableWidget_fis_satirlari->hideColumn  ( KDV_DAHIL_TUTARI_COLUMN                );
    m_ui->tableWidget_fis_satirlari->hideColumn  ( OTV_TUTARI_COLUMN                      );
    m_ui->tableWidget_fis_satirlari->hideColumn  ( OTV_DAHIL_TUTAR_COLUMN                 );
    m_ui->tableWidget_fis_satirlari->hideColumn  ( ISKONTO_TUTARI_COLUMN                  );
    m_ui->tableWidget_fis_satirlari->hideColumn  ( TOPLU_ISKONTO_YUZDESI_COLUMN           );
    m_ui->tableWidget_fis_satirlari->hideColumn  ( T_IS_SONRASI_BIRIM_FIYAT_COLUMN        );
    m_ui->tableWidget_fis_satirlari->hideColumn  ( T_IS_SONRASI_SATIR_TUTARI_COLUMN       );
    m_ui->tableWidget_fis_satirlari->hideColumn  ( T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN  );
    m_ui->tableWidget_fis_satirlari->hideColumn  ( T_IS_SONRASI_OTV_TUTARI_COLUMN         );
    m_ui->tableWidget_fis_satirlari->hideColumn  ( T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN    );
    m_ui->tableWidget_fis_satirlari->hideColumn  ( T_IS_SONRASI_KDV_TUTARI_COLUMN         );
    m_ui->tableWidget_fis_satirlari->hideColumn  ( T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN    );

    if ( KULLANICI_YETKILERI->irs_satir_iskontosu_yapabilir_mi EQ 0 ) {
        m_ui->tableWidget_fis_satirlari->hideColumn ( ISKONTO_YUZDESI_COLUMN );
        m_ui->tableWidget_fis_satirlari->hideColumn ( ISK_SONRASI_BIRIM_FIYAT_COLUMN );
    }


    m_ui->tableWidget_fis_satirlari->hideColumn ( FATURA_SATIRI_SECIMI_COLUMN        );
    m_ui->tableWidget_fis_satirlari->hideColumn ( IADE_IRSALIYE_SATIRI_SECIMI_COLUMN );

    if ( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
        m_ui->tableWidget_fis_satirlari->showColumn ( IADE_IRSALIYE_SATIRI_SECIMI_COLUMN );
    }

    switch ( M_FIS->fis_turu ) {

        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS             :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS            :

            m_ui->tableWidget_fis_satirlari->showColumn     ( FATURA_SATIRI_SECIMI_COLUMN );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( FATURA_SATIRI_SECIMI_COLUMN              , 50  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( IADE_IRSALIYE_SATIRI_SECIMI_COLUMN       , 50  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( URUN_KOD_COLUMN                          , 200 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( URUN_AD_COLUMN                           , 350 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( SERI_NO_PARTI_KODU_COLUMN                , 75 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( STOK_MIKTARI_COLUMN                      , 70  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( URUN_BIRIMI_COLUMN                       , 80  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( ISK_ONCESI_BIRIM_FIYAT_COLUMN            , 150 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( ISK_SONRASI_BIRIM_FIYAT_COLUMN           , 150 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( ISKONTO_YUZDESI_COLUMN                   , 80  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( KDV_DAHIL_MI_COLUMN                      , 65  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( KDV_ORANI_COLUMN                         , 60  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( OTV_ORANI_COLUMN                         , 60  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( SATIR_TUTARI_COLUMN                      , 100 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( PARA_BIRIMI_COLUMN                       , 60  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( VERGI_HARIC_TUTARI_COLUMN                , 100 );

            SET_SETTING_NAME ( "STK_IRS_ELLE_FATURA_IRSALIYELESTIRME" );
            break;
        case ENUM_STK_DEPO_TRANSFER_GIRIS    :
        case ENUM_STK_DEPO_TRANSFER_CIKIS    :
        case ENUM_STK_IMALATA_SEVK_FISI      :
        case ENUM_STK_IMALATTAN_DONUS_FISI   :
        case ENUM_IRS_FASON_BASLANGIC_CIKIS  :
        case ENUM_IRS_FASON_BASLANGIC_GIRIS  :
        case ENUM_IRS_FASON_BITIS_CIKIS      :
        case ENUM_IRS_FASON_BITIS_GIRIS      :

            m_ui->tableWidget_fis_satirlari->setColumnWidth ( FATURA_SATIRI_SECIMI_COLUMN          , 50  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( IADE_IRSALIYE_SATIRI_SECIMI_COLUMN   , 50  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( URUN_KOD_COLUMN                      , 200 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( URUN_AD_COLUMN                       , 500 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( SERI_NO_PARTI_KODU_COLUMN            , 75  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( STOK_MIKTARI_COLUMN                  , 80  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( URUN_BIRIMI_COLUMN                   , 75  );

            m_ui->tableWidget_fis_satirlari->hideColumn  ( ISK_ONCESI_BIRIM_FIYAT_COLUMN                 );
            m_ui->tableWidget_fis_satirlari->hideColumn  ( ISK_SONRASI_BIRIM_FIYAT_COLUMN                );
            m_ui->tableWidget_fis_satirlari->hideColumn  ( ISKONTO_YUZDESI_COLUMN                        );
            m_ui->tableWidget_fis_satirlari->hideColumn  ( KDV_DAHIL_MI_COLUMN                           );
            m_ui->tableWidget_fis_satirlari->hideColumn  ( KDV_ORANI_COLUMN                              );
            m_ui->tableWidget_fis_satirlari->hideColumn  ( OTV_ORANI_COLUMN                              );
            m_ui->tableWidget_fis_satirlari->hideColumn  ( SATIR_TUTARI_COLUMN                           );
            m_ui->tableWidget_fis_satirlari->hideColumn  ( PARA_BIRIMI_COLUMN                            );
            m_ui->tableWidget_fis_satirlari->hideColumn  ( VERGI_HARIC_TUTARI_COLUMN                     );

            m_ui->frame_vergi_bilgileri->setVisible( false );

            m_ui->label_doviz_bilgileri->setVisible( false );
            m_ui->textEdit_doviz_tutar_bilgileri->setVisible( false );

            m_ui->label_satir_bilgileri->setVisible( false );
            m_ui->textEdit_satir_tutar_bilgileri->setVisible( false );

            m_ui->label_kdv_tutarlari->setVisible( false );
            m_ui->textEdit_kdv_yuzdeleri->setVisible( false );

            m_ui->label_otv_tutarlari->setVisible( false );
            m_ui->textEdit_otv_yuzdeleri->setVisible( false );

            m_ui->label_seri_no_or_parti_kodu->setVisible( false );
            m_ui->textEdit_seri_no_bilgileri->setVisible( false );

            SET_SETTING_NAME ( "STK_IRS_ELLE_FATURA_IRSALIYELESTIRME_2" );
        break;

        case ENUM_STK_SARF_FIRE_FISI:
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( FATURA_SATIRI_SECIMI_COLUMN          , 50 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( IADE_IRSALIYE_SATIRI_SECIMI_COLUMN   , 50  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( URUN_KOD_COLUMN                     , 200 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( URUN_AD_COLUMN                      , 350 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( SERI_NO_PARTI_KODU_COLUMN           ,  75 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( STOK_MIKTARI_COLUMN                  , 80  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( URUN_BIRIMI_COLUMN                   , 80  );
            m_ui->tableWidget_fis_satirlari->hideColumn ( ISK_ONCESI_BIRIM_FIYAT_COLUMN );
            m_ui->tableWidget_fis_satirlari->hideColumn ( ISKONTO_YUZDESI_COLUMN );
            m_ui->tableWidget_fis_satirlari->hideColumn ( ISK_SONRASI_BIRIM_FIYAT_COLUMN );
            m_ui->tableWidget_fis_satirlari->hideColumn ( KDV_DAHIL_MI_COLUMN );
            m_ui->tableWidget_fis_satirlari->hideColumn ( KDV_ORANI_COLUMN );
            m_ui->tableWidget_fis_satirlari->hideColumn ( OTV_ORANI_COLUMN );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( SATIR_TUTARI_COLUMN                  , 100  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( PARA_BIRIMI_COLUMN                   , 50  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( VERGI_HARIC_TUTARI_COLUMN            , 100  );

        break;
        default :

            m_ui->tableWidget_fis_satirlari->setColumnWidth ( FATURA_SATIRI_SECIMI_COLUMN          , 50 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( IADE_IRSALIYE_SATIRI_SECIMI_COLUMN   , 50  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( URUN_KOD_COLUMN                     , 200 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( URUN_AD_COLUMN                      , 350 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( SERI_NO_PARTI_KODU_COLUMN           ,  75 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( STOK_MIKTARI_COLUMN                  , 80  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( URUN_BIRIMI_COLUMN                   , 80  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( ISK_ONCESI_BIRIM_FIYAT_COLUMN        , 140 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( ISKONTO_YUZDESI_COLUMN               , 80  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( ISK_SONRASI_BIRIM_FIYAT_COLUMN       , 140 );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( KDV_DAHIL_MI_COLUMN                  , 75  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( KDV_ORANI_COLUMN                     , 75  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( OTV_ORANI_COLUMN                     , 75  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( SATIR_TUTARI_COLUMN                  , 100  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( PARA_BIRIMI_COLUMN                   , 50  );
            m_ui->tableWidget_fis_satirlari->setColumnWidth ( VERGI_HARIC_TUTARI_COLUMN            , 100  );

            SET_SETTING_NAME ( "DIGER_TUM_STOK_FISLERI" );
            break;
    }
    m_ui->frame_irsaliyenin_faturalari->setVisible  ( false );

    switch ( M_FIS->fis_turu ) {

        case ENUM_STK_STOK_ACILIS_FISI  :
        case ENUM_STK_SARF_FIRE_FISI    :

            m_ui->label_cari_hesap->setVisible              ( false );
            m_ui->lineEdit_cari_hesap_ismi->setVisible      ( false );
            m_ui->frame_adres_bilgileri->setVisible         ( false );
            m_ui->searchEdit_cari_hesap_kodu->setVisible    ( false );

            m_ui->label_alan_depo->setVisible               ( false );
            m_ui->searchEdit_alan_depo_kodu->setVisible     ( false );
            m_ui->lineEdit_alan_depo_adi->setVisible        ( false );

            m_ui->adakDate_sevk_tarihi->setVisible          ( false );
            m_ui->label_sevk_tarihi->setVisible             ( false );
            m_ui->lineEdit_belge_numarasi->setVisible   ( false );
            m_ui->lineEdit_irsaliye_belge_seri->setVisible  ( false );
            m_ui->label_irsaliye_seri_no->setVisible        ( false );

            m_ui->label_depo->setText       ("Depo Kodu");
            m_ui->label_fis_tarihi->setText (tr("Fiş Tarihi"));

            m_ui->adakDate_fis_tarihi->setDisabled(false);

            if ( M_FIS->fis_turu EQ ENUM_STK_STOK_ACILIS_FISI ) {
                m_ui->adakDate_fis_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
                m_ui->adakDate_fis_tarihi->setDisabled ( true );
                m_ui->label_bilgilendirme->setHidden(false);
            }

            m_ui->comboBox_vergi_dairesi->setHidden( true );
            m_ui->lineEdit_vergi_no->setHidden     ( true );
            m_ui->label_vergi_dairesi->setHidden   ( true );

            break;

        case ENUM_STK_DEPO_TRANSFER_GIRIS    :
        case ENUM_STK_DEPO_TRANSFER_CIKIS    :

            m_ui->label_cari_hesap->setVisible           ( false );
            m_ui->lineEdit_cari_hesap_ismi->setVisible   ( false );
            m_ui->searchEdit_cari_hesap_kodu->setVisible ( false );
            m_ui->frame_adres_bilgileri->setVisible      ( false );

            m_ui->label_depo->setText                   ( tr ( "Gönderen Depo Kodu" ) );
            m_ui->label_alan_depo->setVisible           ( true );
            m_ui->searchEdit_alan_depo_kodu->setVisible ( true );
            m_ui->lineEdit_alan_depo_adi->setVisible    ( true );

            m_ui->adakDate_fis_tarihi->setDisabled( false );

            m_ui->comboBox_vergi_dairesi->setVisible( false );
            m_ui->lineEdit_vergi_no->setVisible( false );
            m_ui->label_vergi_dairesi->setVisible( false );

            break;

        case ENUM_STK_IMALATA_SEVK_FISI      :
        case ENUM_STK_IMALATTAN_DONUS_FISI   :
        case ENUM_IRS_FASON_BASLANGIC_CIKIS  :
        case ENUM_IRS_FASON_BASLANGIC_GIRIS  :
        case ENUM_IRS_FASON_BITIS_CIKIS      :    
        case ENUM_IRS_FASON_BITIS_GIRIS      : {

            bool cari_bilgileri_is_visible = false;
            if ( M_FIS->fis_turu EQ ENUM_IRS_FASON_BASLANGIC_CIKIS  OR
                 M_FIS->fis_turu EQ ENUM_IRS_FASON_BASLANGIC_GIRIS  OR
                 M_FIS->fis_turu EQ ENUM_IRS_FASON_BITIS_CIKIS      OR
                 M_FIS->fis_turu EQ ENUM_IRS_FASON_BITIS_GIRIS   ) {

                cari_bilgileri_is_visible = true;
            }
            m_ui->label_cari_hesap->setVisible              ( cari_bilgileri_is_visible );
            m_ui->lineEdit_cari_hesap_ismi->setVisible      ( cari_bilgileri_is_visible );
            m_ui->searchEdit_cari_hesap_kodu->setVisible    ( cari_bilgileri_is_visible );
            m_ui->frame_adres_bilgileri->setVisible         ( cari_bilgileri_is_visible );

            m_ui->comboBox_vergi_dairesi->setVisible        ( cari_bilgileri_is_visible );
            m_ui->lineEdit_vergi_no->setVisible             ( cari_bilgileri_is_visible );
            m_ui->label_vergi_dairesi->setVisible           ( cari_bilgileri_is_visible );

            m_ui->label_irsaliye_seri_no->setVisible        ( true );
            m_ui->lineEdit_belge_numarasi->setVisible       ( true );
            m_ui->lineEdit_irsaliye_belge_seri->setVisible  ( true );


            if ( M_FIS->fis_turu EQ ENUM_STK_IMALATA_SEVK_FISI OR M_FIS->fis_turu EQ ENUM_STK_IMALATTAN_DONUS_FISI ) {
                m_ui->label_irsaliye_seri_no->setHidden       ( true );
                m_ui->lineEdit_irsaliye_belge_seri->setHidden ( true );
                m_ui->lineEdit_belge_numarasi->setHidden      ( true );
            }

            m_ui->label_alan_depo->setVisible            ( false );
            m_ui->searchEdit_alan_depo_kodu->setVisible  ( false );
            m_ui->lineEdit_alan_depo_adi->setVisible     ( false );

            m_ui->label_depo->setText(tr("Depo Kodu"));

            m_ui->adakDate_fis_tarihi->setDisabled(false);

            break;
        }
        case ENUM_IRS_KONSINYE_CIKIS                     :
        case ENUM_IRS_NORMAL_CIKIS                       :
        case ENUM_IRS_NORMAL_GIRIS                       :
        case ENUM_IRS_KONSINYE_GIRIS                     :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS  :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS :
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS   :
        case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS  :

            if ( M_FIS->fis_turu EQ ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS  OR
                 M_FIS->fis_turu EQ ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS OR
                 M_FIS->fis_turu EQ ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS   OR
                 M_FIS->fis_turu EQ ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS ) {
            }

            GET_VERGI_DAIRELERI   ( m_ui->comboBox_vergi_dairesi );

            m_ui->label_alan_depo->setVisible           ( false );
            m_ui->searchEdit_alan_depo_kodu->setVisible ( false );
            m_ui->lineEdit_alan_depo_adi->setVisible    ( false );

            m_ui->label_depo->setText(tr("Depo Kodu"));

            m_ui->adakDate_fis_tarihi->setDisabled(false);

            if ( M_FIS->fis_turu EQ ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS OR
                 M_FIS->fis_turu EQ ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS ) {

                m_ui->frame_irsaliyenin_faturalari->setVisible(true);
            }
            break;
    }

    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {

        if ( M_FIS->irsaliye_alis_satis_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI) {
            if ( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
                SET_PAGE_TITLE ( "İRS - SATIŞTAN İADE İRSALİYELERİ" );
                SET_HELP_PAGE("irsaliye-islemleri_satistan-iade-irsaliyeleri");
            }
            else {
                SET_PAGE_TITLE ( "İRS - ALIŞ İRSALİYELERİ" );
                SET_HELP_PAGE("irsaliye-islemleri_alis-irsaliyeleri");
            }
        }
        else {
            if ( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
                SET_PAGE_TITLE ( "İRS - ALIŞTAN İADE İRSALİYELERİ" );
                SET_HELP_PAGE("irsaliye-islemleri_alistan-iade-irsaliyeleri");
            }
            else {
                SET_PAGE_TITLE ( "İRS - SATIŞ İRSALİYELERİ" );
                SET_HELP_PAGE("irsaliye-islemleri_satis-irsaliyeleri");
            }
        }

        switch( M_FIS->fis_turu ) {
            case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS:
                SET_PAGE_TITLE ( "İRS - FATURA KISMEN İRSALİYELEŞTİRME ( ALIŞ )" );
                SET_HELP_PAGE("irsaliye-islemleri_satis-fatura-kismen-irsaliyelestirme");
                break;
            case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS:
                SET_PAGE_TITLE ( "İRS - FATURA KISMEN İRSALİYELEŞTİRME ( SATIŞ )" );
                SET_HELP_PAGE("irsaliye-islemleri_satis-fatura-kismen-irsaliyelestirme");
                break;
            case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_ALIS:
                SET_PAGE_TITLE ( "İRS - FATURA İRSALİYELEŞTİRME ( ALIŞ )" );
                SET_HELP_PAGE("irsaliye-islemleri_satis-fatura-irsaliyelestirme");
                break;
            case ENUM_IRS_OTO_FATURA_IRSALIYELESTIRME_SATIS:
                SET_PAGE_TITLE ( "İRS - FATURA İRSALİYELEŞTİRME ( SATIŞ )" );
                SET_HELP_PAGE("irsaliye-islemleri_satis-fatura-irsaliyelestirme");
                break;
            case ENUM_IRS_FASON_BASLANGIC_GIRIS:
                SET_PAGE_TITLE ( "İRS - FASON BAŞLANGIÇ İRSALİYESİ GELEN" );
                SET_HELP_PAGE("irsaliye-islemleri_giden-fason-bitis");
                break;
            case ENUM_IRS_FASON_BASLANGIC_CIKIS:
                SET_PAGE_TITLE ( "İRS - FASON BAŞLANGIÇ İRSALİYESİ GİDEN" );
                SET_HELP_PAGE("irsaliye-islemleri_giden-fason-baslangic");
                break;
            case ENUM_IRS_FASON_BITIS_GIRIS:
                SET_PAGE_TITLE ( "İRS - FASON BİTİŞ İRSALİYESİ GELEN" );
                SET_HELP_PAGE("irsaliye-islemleri_gelen-fason-bitis");
                break;
            case ENUM_IRS_FASON_BITIS_CIKIS:
                SET_PAGE_TITLE ( "İRS - FASON BİTİŞ İRSALİYESİ GİDEN" );
                SET_HELP_PAGE("irsaliye-islemleri_gelen-fason-baslangic");
                break;

            default:
                break;
        }
    }
    else {

        switch ( M_FIS->fis_turu ) {

            case ENUM_STK_SARF_FIRE_FISI :
                SET_HELP_PAGE ( "stok-islemleri_sarffire-fisi" );
                break;
            case ENUM_STK_IMALATA_SEVK_FISI :
                 SET_HELP_PAGE ( "stok-islemleri_imalata-sevk-fisi" );
                break;
            case ENUM_STK_IMALATTAN_DONUS_FISI :
                SET_HELP_PAGE ( "stok-islemleri_imalattan-giris-fisi" );
                break;
            case ENUM_STK_STOK_ACILIS_FISI :
                SET_HELP_PAGE("stok-islemleri_acilis-fisi");
                break;
            default :
                break;
        }

        SET_PAGE_TITLE( STK_GET_STOK_FIS_TURU( M_FIS->fis_turu ) );

    }

    bool sube_is_visible = true;
    if( M_FIS->fis_turu NE ENUM_STK_DEPO_TRANSFER_GIRIS AND M_FIS->fis_turu NE ENUM_STK_DEPO_TRANSFER_CIKIS ) {
        if( SUBE_GET_DEPO_COUNT( SUBE_GET_SUBE_ID() ) > 1 ) {
            sube_is_visible = true;
        }
        else {
            sube_is_visible = false;
        }
    }


    m_ui->searchEdit_depo_kodu->setVisible( sube_is_visible );
    m_ui->label_depo->setVisible( sube_is_visible );
    m_ui->lineEdit_depo_adi->setVisible( sube_is_visible );

}

/**************************************************************************************
                   STOK_FISI::URUN_BILGILERINI_AL
***************************************************************************************/

void STOK_FISI::URUN_BILGILERINI_AL ( int p_row_number )
{
    QLineEdit  * lineEdit_urun_id = ( QLineEdit  *  ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN );

    int urun_id = lineEdit_urun_id->text().toInt();

    if ( urun_id < 1 ) {
        return;
    }

    if ( m_last_urun_id NE urun_id ) {

        m_last_urun_id = urun_id;

        SQL_QUERY query ( DB );

        query.PREPARE_SELECT ( "stk_urunler",
                               "urun_birim_katsayilari_array, urun_capraz_birimler_array, "
                               "urun_birim_alis_fiyatlar_array, urun_birim_sts_fiyatlar_array ",
                               "urun_id = :urun_id");
        query.SET_VALUE      ( ":urun_id", urun_id );

        if ( query.SELECT() EQ 0 ) {
            return;
        }

        query.NEXT();

        QString urun_birim_katsayilari_str      = query.VALUE(0).toString();
        QString urun_capraz_birimler_str        = query.VALUE(1).toString();
        QString urun_birim_alis_fiyatlar_str   = query.VALUE(2).toString();
        QString urun_birim_sts_fiyatlar_str  = query.VALUE(3).toString();

        UNPACK_DOUBLE_ARRAY ( urun_birim_katsayilari_str,       M_STOK_BIRIM_KATSAYILARI,     URUN_BIRIMLERI_SAYISI );
        UNPACK_INT_ARRAY    ( urun_capraz_birimler_str,         M_STOK_CAPRAZ_BIRIMLER,       URUN_BIRIMLERI_SAYISI );
        UNPACK_DOUBLE_ARRAY ( urun_birim_alis_fiyatlar_str,    M_STOK_BIRIM_ALIS_FIYATLARI,  URUN_BIRIMLERI_SAYISI );
        UNPACK_DOUBLE_ARRAY ( urun_birim_sts_fiyatlar_str,   M_STOK_BIRIM_SATIS_FIYATLARI, URUN_BIRIMLERI_SAYISI );
    }

}



/**************************************************************************************
                   STOK_FISI::TEMEL_BIRIM_CINSINDEN_KATSAYI_HESAPLA
***************************************************************************************/

void STOK_FISI::TEMEL_BIRIM_CINSINDEN_KATSAYI_HESAPLA ( int p_row_number )
{

    QLineEdit  * lineEdit_urun_id                = ( QLineEdit  *  ) m_ui->tableWidget_fis_satirlari->cellWidget   ( p_row_number, URUN_ID_COLUMN );
    QComboBox  * comboBox_stok_birimi            = ( QComboBox  *  ) m_ui->tableWidget_fis_satirlari->cellWidget   ( p_row_number, URUN_BIRIMI_COLUMN );
    QCommaEdit * commaEdit_temel_birim_katsayisi = ( QCommaEdit *  ) m_ui->tableWidget_fis_satirlari->cellWidget   ( p_row_number, TEMEL_BIRIM_KATSAYISI );

    int urun_id = lineEdit_urun_id->text().toInt();

    if ( urun_id < 1 ) {
        return;
    }

    double katsayi = 1.00;

    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
        if ( M_STOK_BIRIM_ADLARI[i] NE comboBox_stok_birimi->currentText() ) {
            continue;
        }
        katsayi              = 1.0;
        int    capraz_birim  = M_STOK_CAPRAZ_BIRIMLER[i];
        katsayi              = ROUND ( katsayi * M_STOK_BIRIM_KATSAYILARI[i] );
        while ( capraz_birim > 0 ) {
            katsayi      = ROUND ( katsayi * M_STOK_BIRIM_KATSAYILARI[capraz_birim -1 ] );
            capraz_birim = M_STOK_CAPRAZ_BIRIMLER  [capraz_birim -1 ];
        }
        break;
    }

    commaEdit_temel_birim_katsayisi->SET_DOUBLE( katsayi );

}

/**************************************************************************************
                   STOK_FISI::URUN_BIRIMI_FIYATLARINI_HESAPLA
***************************************************************************************/

void STOK_FISI::URUN_BIRIMI_FIYATLARINI_HESAPLA ( int p_row_number )
{
    QComboBox  * comboBox_stok_birimi               = ( QComboBox  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN               );
    QCommaEdit * commaEdit_isk_oncesi_birim_fiyat   = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN    );
    QCommaEdit * commaEdit_isk_sonrasi_birim_fiyat  = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN   );
    QCommaEdit * commaEdit_iskonto_yuzdesi          = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISKONTO_YUZDESI_COLUMN           );
    QCommaEdit * commaEdit_iskonto_tutari           = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISKONTO_TUTARI_COLUMN            );
    QCommaEdit * commaEdit_satir_tutari             = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN              );
    QCommaEdit * commaEdit_satir_stok_miktari       = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_MIKTARI_COLUMN              );
    QLineEdit  * lineEdit_urun_id                   = ( QLineEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN                   );
    QComboBox  * comboBox_kdv_orani                 = ( QComboBox  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_ORANI_COLUMN                 );
    QComboBox  * comboBox_otv_orani                 = ( QComboBox  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_ORANI_COLUMN                 );
    QComboBox  * comboBox_kdv_dahil_mi              = ( QComboBox  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN              );
    //QLineEdit  * lineEdit_para_birimi               = ( QLineEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, PARA_BIRIMI_COLUMN               );

    double  satir_tutari = 0.00;

    commaEdit_iskonto_tutari->clear();
    commaEdit_iskonto_yuzdesi->clear();

    if ( comboBox_stok_birimi->currentIndex() EQ 0 ) {

        STK_URUNLER_STRUCT * URUN = new STK_URUNLER_STRUCT;
        STK_URUNLER_TABLOSUNU_OKU(lineEdit_urun_id->text().toInt() , URUN);

        // Alış ve satış fiyatı fiş/irsaliye türüne göre seçilecek

        switch ( M_FIS->irsaliye_alis_satis_turu ) {

            case     ENUM_ALIS_IRSALIYESI_FATURASI :
            default                                : {
                if ( M_FIS->stk_irs_para_birimi_id EQ URUN->urun_alis_doviz_id ) {
                    commaEdit_isk_oncesi_birim_fiyat->SET_DOUBLE ( URUN->alis_fiyati );
                    commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE ( URUN->alis_fiyati );
                }
                else {
                    double urun_fiyati = DVZ_PARA_BIRIMLERINI_DONUSTUR ( URUN->urun_alis_doviz_id, URUN->alis_fiyati, M_FIS->stk_irs_para_birimi_id , m_ui->adakDate_fis_tarihi->QDATE(), ENUM_ALIS );
                    commaEdit_isk_oncesi_birim_fiyat->SET_DOUBLE ( urun_fiyati );
                    commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE ( urun_fiyati );
                }
                break;

            }
            case ENUM_SATIS_IRSALIYESI_FATURASI : {
                if ( M_FIS->stk_irs_para_birimi_id EQ URUN->urun_satis_doviz_id )  {
                    commaEdit_isk_oncesi_birim_fiyat->SET_DOUBLE ( URUN->satis_fiyati );
                    commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE ( URUN->satis_fiyati );
                }
                else {
                    double urun_fiyati = DVZ_PARA_BIRIMLERINI_DONUSTUR ( URUN->urun_satis_doviz_id, URUN->satis_fiyati, M_FIS->stk_irs_para_birimi_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS );
                    commaEdit_isk_oncesi_birim_fiyat->SET_DOUBLE ( urun_fiyati );
                    commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE ( urun_fiyati );
                }
                break;
            }

        }
        satir_tutari = ROUND ( commaEdit_satir_stok_miktari->GET_DOUBLE() * commaEdit_isk_sonrasi_birim_fiyat->GET_DOUBLE() );

        switch ( M_FIS->fis_turu ) {
        // bazi islemlerde fiyat getirmesine gerek yok mesela depoya mal gonderme
            case ENUM_STK_DEPO_TRANSFER_GIRIS    :
            case ENUM_STK_DEPO_TRANSFER_CIKIS    :
            case ENUM_STK_IMALATA_SEVK_FISI      :
            case ENUM_STK_IMALATTAN_DONUS_FISI   :
            case ENUM_IRS_FASON_BASLANGIC_CIKIS  :
            case ENUM_IRS_FASON_BASLANGIC_GIRIS  :
            case ENUM_IRS_FASON_BITIS_CIKIS      :
            case ENUM_IRS_FASON_BITIS_GIRIS      :
                 commaEdit_isk_oncesi_birim_fiyat->clear();
                 commaEdit_isk_sonrasi_birim_fiyat->clear();
                 comboBox_kdv_orani->clear();
                 comboBox_otv_orani->clear();
                 break;
            default :
                break;
        }

    }

    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
        if ( M_STOK_BIRIM_ADLARI[i] NE comboBox_stok_birimi->currentText() ) {
            continue;
        }
        commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE ( M_STOK_BIRIM_ALIS_FIYATLARI [i] );
        commaEdit_isk_oncesi_birim_fiyat->SET_DOUBLE  ( M_STOK_BIRIM_ALIS_FIYATLARI [i] );
        satir_tutari = ROUND ( M_STOK_BIRIM_ALIS_FIYATLARI[i] * commaEdit_satir_stok_miktari->GET_DOUBLE() );
        break;
    }

    double kdv_orani            = comboBox_kdv_orani->currentText().toDouble();
    double otv_orani            = comboBox_otv_orani->currentText().toDouble();
    double vergi_haric_tutar    = 0.00;

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
                   STOK_FISI::SATIR_DOVIZ_TUTARLARINI_GUNCELLE
***************************************************************************************/

void STOK_FISI::SATIR_DOVIZ_TUTARLARINI_GUNCELLE ( QString p_satir_doviz_kodu, double p_satir_doviz_tutari, int p_ekle_sil )
{
    QStringList SATIR_BILGILERI;

    double      doviz_toplami      = 0.00;
    bool        doviz_kodu_bulundu = false;

    if ( p_satir_doviz_tutari EQ 0 ) {
        return;
    }

    for ( int i = 0; i < M_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI.size(); i++ ) {
        SATIR_BILGILERI       = M_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI.at(i).split("\t");

        doviz_toplami      = SATIR_BILGILERI.at(0).toDouble();
        QString doviz_kodu = SATIR_BILGILERI.at(1);
        if ( doviz_kodu EQ p_satir_doviz_kodu ) {
            doviz_kodu_bulundu = true;

            switch ( p_ekle_sil ) {

                case DOVIZ_EKLE :
                    doviz_toplami += p_satir_doviz_tutari;
                    M_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI[i] = QVariant(doviz_toplami).toString() + "\t" + p_satir_doviz_kodu;
                    return;

                case DOVIZ_SIL :
                    doviz_toplami -= p_satir_doviz_tutari;
                    if ( doviz_toplami <= 0 ) {
                        M_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI.removeAt(i);
                    }
                    else {
                        M_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI[i] = QVariant(doviz_toplami).toString() + "\t" + p_satir_doviz_kodu;
                    }
                    return;

                default :
                    return;
            }
        }
    }
    if ( doviz_kodu_bulundu EQ false ) {
        if ( p_ekle_sil EQ DOVIZ_EKLE ) {
            QString satir_bilgileri = QVariant( p_satir_doviz_tutari ).toString() + "\t" + p_satir_doviz_kodu;
            M_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI.append(satir_bilgileri);
        }
    }
}



/**************************************************************************************
                   STOK_FISI::SATIR_DOVIZ_TUTARLARINI_YAZDIR
***************************************************************************************/

void STOK_FISI::SATIR_DOVIZ_TUTARLARINI_YAZDIR ()
{
    QStringList SATIR_BILGILERI;
    QString     print_text;

    m_ui->textEdit_doviz_tutar_bilgileri->clear();

    for ( int i = 0; i < M_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI.size(); i++ ) {
            SATIR_BILGILERI = M_DOVIZ_CINSINDEN_SATIR_TOPLAMLARI[i].split("\t");
            print_text   += VIRGUL_EKLE ( QVariant ( ROUND( SATIR_BILGILERI.at(0).toDouble() )).toString() , 2 ).rightJustified(8) + " " + SATIR_BILGILERI.at(1) + "\n" ;
    }
    m_ui->textEdit_doviz_tutar_bilgileri->setText(print_text);
}

/**************************************************************************************
                   STOK_FISI::KDV_ORANLARI_BILGILERINI_GUNCELLE
***************************************************************************************/

void STOK_FISI::KDV_ORANLARI_BILGILERINI_GUNCELLE ( QString p_satir_kdv_orani, double  p_satir_kdv_tutari, int p_ekle_sil )
{
    QStringList KDV_BILGILERI;

    double      kdv_tutar_toplami = 0.00;
    bool        kdv_orani_bulundu = false;

    if ( p_satir_kdv_orani.toDouble() EQ 0 ) {
        return;
    }

    for ( int i = 0; i < M_KDV_ORANLARINA_GORE_KDV_TUTARLARI.size(); i++ ) {
        KDV_BILGILERI       = M_KDV_ORANLARINA_GORE_KDV_TUTARLARI.at(i).split("\t");
        QString kdv_orani   = KDV_BILGILERI.at(0).mid(1);
        kdv_tutar_toplami   = KDV_BILGILERI.at(1).toDouble();
        if ( kdv_orani EQ p_satir_kdv_orani ) {
            kdv_orani_bulundu = true;

            switch ( p_ekle_sil ) {

                case VERGI_ORANI_EKLE :
                    kdv_tutar_toplami += p_satir_kdv_tutari;
                    M_KDV_ORANLARINA_GORE_KDV_TUTARLARI[i] = "%" + kdv_orani + "\t" + QVariant(kdv_tutar_toplami).toString();
                    return;

                case VERGI_ORANI_SIL :
                    kdv_tutar_toplami -= p_satir_kdv_tutari;
                    if ( kdv_tutar_toplami <= 0 ) {
                        M_KDV_ORANLARINA_GORE_KDV_TUTARLARI.removeAt(i);
                    }
                    else {
                        M_KDV_ORANLARINA_GORE_KDV_TUTARLARI[i] = "%" + kdv_orani + "\t" + QVariant(kdv_tutar_toplami).toString();
                    }
                    return;

                default :
                    return;
            }
        }
    }
    if ( kdv_orani_bulundu EQ false ) {
        if ( p_ekle_sil EQ VERGI_ORANI_EKLE ) {
            QString satir_bilgileri = "%" + p_satir_kdv_orani + "\t" + QVariant ( p_satir_kdv_tutari ).toString();
            M_KDV_ORANLARINA_GORE_KDV_TUTARLARI.append(satir_bilgileri);
        }
    }
}

/**************************************************************************************
                   STOK_FISI::KDV_ORANLARI_BILGILERINI_YAZDIR
***************************************************************************************/

void STOK_FISI::KDV_ORANLARI_BILGILERINI_YAZDIR ()
{
    QStringList KDV_BILGILERI;
    QString     print_text;

    m_ui->textEdit_kdv_yuzdeleri->clear();

    for ( int i = 0; i < M_KDV_ORANLARINA_GORE_KDV_TUTARLARI.size(); i++ ) {
            KDV_BILGILERI = M_KDV_ORANLARINA_GORE_KDV_TUTARLARI[i].split("\t");
            print_text   += KDV_BILGILERI.at(0) + " " + VIRGUL_EKLE ( QVariant ( ROUND( KDV_BILGILERI.at(1).toDouble() )).toString() , 2 ).rightJustified(8) + " " + m_temel_para_birimi_kodu + "\n" ;
    }
    m_ui->textEdit_kdv_yuzdeleri->setText(print_text);
}

/**************************************************************************************
                   STOK_FISI::OTV_ORANLARI_BILGILERINI_GUNCELLE
***************************************************************************************/

void STOK_FISI::OTV_ORANLARI_BILGILERINI_GUNCELLE ( QString p_satir_otv_orani, double  p_satir_otv_tutari, int p_ekle_sil )
{
    QStringList OTV_BILGILERI;

    double      otv_tutar_toplami = 0.00;
    bool        otv_orani_bulundu = false;

    if ( p_satir_otv_orani.toDouble() EQ 0 ) {
        return;
    }

    for ( int i = 0; i < M_OTV_ORANLARINA_GORE_OTV_TUTARLARI.size(); i++ ) {
        OTV_BILGILERI       = M_OTV_ORANLARINA_GORE_OTV_TUTARLARI.at(i).split("\t");
        QString otv_orani   = OTV_BILGILERI.at(0).mid(1);
        otv_tutar_toplami   = OTV_BILGILERI.at(1).toDouble();
        if ( otv_orani EQ p_satir_otv_orani ) {
            otv_orani_bulundu = true;

            switch ( p_ekle_sil ) {

                case VERGI_ORANI_EKLE :
                    otv_tutar_toplami += p_satir_otv_tutari;
                    M_OTV_ORANLARINA_GORE_OTV_TUTARLARI[i] = "%" + otv_orani + "\t" + QVariant ( otv_tutar_toplami).toString();
                    return;

                case VERGI_ORANI_SIL :
                    otv_tutar_toplami -= p_satir_otv_tutari;
                    if ( otv_tutar_toplami <= 0 ) {
                        M_OTV_ORANLARINA_GORE_OTV_TUTARLARI.removeAt(i);
                    }
                    else {
                        M_OTV_ORANLARINA_GORE_OTV_TUTARLARI[i] = "%" + otv_orani + "\t" + QVariant ( otv_tutar_toplami).toString();
                    }
                    return;

                default :
                    return;
            }
        }
    }
    if ( otv_orani_bulundu EQ false ) {
        if ( p_ekle_sil EQ VERGI_ORANI_EKLE ) {
            QString satir_bilgileri = "%" + p_satir_otv_orani + "\t" + QVariant ( p_satir_otv_tutari ).toString();
            M_OTV_ORANLARINA_GORE_OTV_TUTARLARI.append ( satir_bilgileri );
        }
    }
}

/**************************************************************************************
                   STOK_FISI::OTV_ORANLARI_BILGILERINI_YAZDIR
***************************************************************************************/

void STOK_FISI::OTV_ORANLARI_BILGILERINI_YAZDIR ()
{
    QStringList OTV_BILGILERI;
    QString     print_text;

    m_ui->textEdit_otv_yuzdeleri->clear();

    for ( int i = 0; i < M_OTV_ORANLARINA_GORE_OTV_TUTARLARI.size(); i++ ) {
            OTV_BILGILERI = M_OTV_ORANLARINA_GORE_OTV_TUTARLARI[i].split("\t");
            print_text   += OTV_BILGILERI.at(0) + " "  + VIRGUL_EKLE ( QVariant ( ROUND( OTV_BILGILERI.at(1).toDouble() )).toString() , 2 ).rightJustified(8) + " " + m_temel_para_birimi_kodu + "\n" ;
    }
    m_ui->textEdit_otv_yuzdeleri->setText ( print_text );
}

/**************************************************************************************
                   STOK_FISI::LINE_SEARCH_EDIT_CLICKED
***************************************************************************************/

void STOK_FISI::LINE_SEARCH_EDIT_CLICKED ( int p_row_number, QWidget * p_widget, QLineEdit * p_lineEdit)
{
    if ( M_FIS->depo_id < 1 ) {
        MSG_WARNING(  tr("Önce depo seçimi yapmalısınız."), NULL );
        return;
    }

    QSearchEdit  * searchEdit_urun_kodu   = ( QSearchEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_KOD_COLUMN );

    SQL_QUERY query ( DB );

    int urun_id = 0;

    if ( p_widget EQ searchEdit_urun_kodu ) {

        switch ( M_FIS->irsaliye_alis_satis_turu ) {

            case ENUM_ALIS_IRSALIYESI_FATURASI :
            default                            :
                urun_id = OPEN_STOK_URUN_ARAMA ( ENUM_URUN_KARTI,-1, p_lineEdit->text(),-1,this );
                break;
            case ENUM_SATIS_IRSALIYESI_FATURASI :
                urun_id = OPEN_STOK_URUN_ARAMA ( ENUM_URUN_KARTI,M_FIS->depo_id, p_lineEdit->text(),-1, this );
                break;
        }

        if ( urun_id < 1 ) {
            return;
        }
        switch ( M_FIS->fis_turu ) {

            case ENUM_ALIS_IRSALIYESI_FATURASI :
            default                         :
                query.PREPARE_SELECT ( "stk_urunler","urun_kodu",
                                       "urun_id = :urun_id " );
                query.SET_VALUE(":urun_id", urun_id   );
                break;

            case ENUM_SATIS_IRSALIYESI_FATURASI :

                query.PREPARE_SELECT ( "stk_depodaki_urunler, stk_urunler","urun_kodu",
                                       "stk_depodaki_urunler.urun_id = stk_urunler.urun_id "
                                       "AND stk_depodaki_urunler.urun_id = :urun_id AND depo_id = :depo_id " );
                query.SET_VALUE(":depo_id", M_FIS->depo_id );
                query.SET_VALUE(":urun_id", urun_id   );
                break;
        }

        if ( query.SELECT() EQ 0 ) {
            return;
        }
        query.NEXT();
        p_lineEdit->setText(query.VALUE(0).toString());

    }

}

/**************************************************************************************
                   STOK_FISI::SEARCH_EDIT_CLICKED
***************************************************************************************/

void STOK_FISI::SEARCH_EDIT_CLICKED ( QWidget * p_widget, QLineEdit * p_lineEdit)
{

    if ( p_widget EQ m_ui->searchEdit_depo_kodu OR p_widget EQ m_ui->searchEdit_alan_depo_kodu ) {

        int depo_id = SUBE_DEPO_SEC ( SUBE_GET_SUBE_ID(), this );

        if ( depo_id < 1 ) {
            return;
        }

        QString depo_kodu = SUBE_GET_DEPO_KODU( depo_id );

        if ( depo_kodu EQ 0 ) {
            return;
        }
        p_lineEdit->setText( depo_kodu );
    }
    else if ( p_widget EQ m_ui->searchEdit_cari_hesap_kodu ) {

        int cari_hesap_id = OPEN_CARI_KART_ARAMA ( p_lineEdit->text(), SAHIS_ARTI_FIRMA, this, 1 , E9_PROGRAMI, CARI_MODULU );

        if ( cari_hesap_id < 1 ) {
            return;
        }

        QString cari_hesap_kodu;
        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( cari_hesap_id, &cari_hesap_kodu );

        if ( result EQ 0 ) {
            return;
        }
        p_lineEdit->setText( cari_hesap_kodu );
    }
}

/**************************************************************************************
                   STOK_FISI::SELECTED_LINE
***************************************************************************************/

void STOK_FISI::SELECTED_LINE ( int p_row_number, int p_row_save_status )
{
    Q_UNUSED ( p_row_save_status );

    QCommaEdit   * commaEdit_satirdaki_urun_miktari  = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget  ( p_row_number, STOK_MIKTARI_COLUMN       );
    QCommaEdit   * commaEdit_vergi_haric_tutar       = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget  ( p_row_number, VERGI_HARIC_TUTARI_COLUMN       );
    QCommaEdit   * commaEdit_kdv_tutari              = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget  ( p_row_number, KDV_TUTAR_COLUMN              );
    QCommaEdit   * commaEdit_kdv_dahil_tutar         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget  ( p_row_number, KDV_DAHIL_TUTARI_COLUMN         );
    QComboBox    * comboBox_kdv_dahil_mi             = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget  ( p_row_number, KDV_DAHIL_MI_COLUMN            );
    QCommaEdit   * commaEdit_isk_sonrasi_birim_fiyat = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget  ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN );
    QComboBox    * comboBox_kdv_orani                = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget  ( p_row_number, KDV_ORANI_COLUMN               );
    QComboBox    * comboBox_otv_orani                = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget  ( p_row_number, OTV_ORANI_COLUMN               );
    QCommaEdit   * commaEdit_otv_tutari              = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget  ( p_row_number, OTV_TUTARI_COLUMN              );
    QCommaEdit   * commaEdit_otv_dahil_tutar         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget  ( p_row_number, OTV_DAHIL_TUTAR_COLUMN         );
    QLineEdit    * lineEdit_para_birimi              = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget  ( p_row_number, PARA_BIRIMI_COLUMN             );
    QLineEdit    * lineEdit_seri_no_list             = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget  ( p_row_number, SERI_NO_LISTE_COLUMN            );

    QString satir_tutar_bilgileri;

    double satirdaki_urun_miktari                   = commaEdit_satirdaki_urun_miktari->GET_DOUBLE();
    double urun_birim_fiyati                        = commaEdit_isk_sonrasi_birim_fiyat->GET_DOUBLE();
    double kdv_orani                                = comboBox_kdv_orani->currentText().toDouble();
    double vergi_haric_tutar                        = commaEdit_vergi_haric_tutar->GET_DOUBLE();
    double kdv_tutari                               = commaEdit_kdv_tutari->GET_DOUBLE();
    double kdv_dahil_tutar                          = commaEdit_kdv_dahil_tutar->GET_DOUBLE();
    double otv_tutari                               = commaEdit_otv_tutari->GET_DOUBLE();
    double otv_dahil_tutar                          = commaEdit_otv_dahil_tutar->GET_DOUBLE();
    double otv_orani                                = comboBox_otv_orani->currentText().toDouble();
    double kdv_dahil_birim_fiyat                    = 0.00;
    double kdv_haric_birim_fiyat                    = 0.00;
    double otv_dahil_birim_fiyat                    = 0.00;
    double birim_fiyat_kdv_tutari                   = 0.00;
    double birim_fiyat_otv_tutari                   = 0.00;
    double temel_para_birimi_cinsinden_birim_fiyat  = 0.00;

    if ( comboBox_kdv_dahil_mi->currentIndex() EQ 0 ) {
        if ( comboBox_otv_orani->currentText().toInt() > 0 ) {
             otv_tutari  = urun_birim_fiyati * ( otv_orani / 100.0);
        }

        birim_fiyat_kdv_tutari = urun_birim_fiyati * ( kdv_orani / 100.0 ) ;
        kdv_dahil_birim_fiyat  = ( ( urun_birim_fiyati + otv_tutari ) * ( 1 + ( kdv_orani / 100 ) ) );
        temel_para_birimi_cinsinden_birim_fiyat = ROUND ( DVZ_PARA_BIRIMLERINI_DONUSTUR ( DVZ_GET_DOVIZ_ID ( lineEdit_para_birimi->text() ), kdv_dahil_birim_fiyat, m_temel_para_birimi_id, m_ui->adakDate_fis_tarihi->QDATE() )  );
        satir_tutar_bilgileri += tr ( "KDV Dahil Birim Fiyat : " ) +  VIRGUL_EKLE( QVariant ( ROUND( temel_para_birimi_cinsinden_birim_fiyat / satirdaki_urun_miktari ) ).toString() ) + " " + m_temel_para_birimi_kodu + "\n" ;
    }
    else {
        kdv_haric_birim_fiyat                   = urun_birim_fiyati / ( 1 + ( kdv_orani / 100 ) );
        birim_fiyat_kdv_tutari                  = urun_birim_fiyati - kdv_haric_birim_fiyat;
        temel_para_birimi_cinsinden_birim_fiyat = ROUND ( DVZ_PARA_BIRIMLERINI_DONUSTUR ( DVZ_GET_DOVIZ_ID( lineEdit_para_birimi->text() ), kdv_haric_birim_fiyat, m_temel_para_birimi_id, m_ui->adakDate_fis_tarihi->QDATE() ) );
        satir_tutar_bilgileri += tr ( "KDV Hariç Birim Fiyat : " ) + VIRGUL_EKLE( QVariant ( ROUND( temel_para_birimi_cinsinden_birim_fiyat / satirdaki_urun_miktari ) ).toString() ) + " " + m_temel_para_birimi_kodu + "\n" ;

    }

    satir_tutar_bilgileri += tr ( "Satır KDV Hariç Tutar : " )     + VIRGUL_EKLE ( QVariant ( vergi_haric_tutar ).toString()) + " " + m_temel_para_birimi_kodu + "\n";
    satir_tutar_bilgileri += tr ( "Satır KDV Tutarı          : " ) + VIRGUL_EKLE ( QVariant ( kdv_tutari        ).toString() ) + " " + m_temel_para_birimi_kodu + "\n";
    satir_tutar_bilgileri += tr ( "Satır KDV Dahil Tutar : ")      + VIRGUL_EKLE ( QVariant ( kdv_dahil_tutar   ).toString() ) + " " + m_temel_para_birimi_kodu + "\n";
    satir_tutar_bilgileri += tr ( "Satır ÖTV Tutarı          : ")  + VIRGUL_EKLE ( QVariant ( otv_tutari        ).toString() ) + " " + m_temel_para_birimi_kodu + "\n";
    satir_tutar_bilgileri += tr ( "Satır ÖTV Dahil Tutar : ")      + VIRGUL_EKLE ( QVariant ( otv_dahil_tutar   ).toString() ) + " " + m_temel_para_birimi_kodu + "\n";

    birim_fiyat_otv_tutari  = birim_fiyat_kdv_tutari * ( otv_orani / 100 );

    if ( comboBox_kdv_dahil_mi->currentIndex() EQ 0 ) {
        otv_dahil_birim_fiyat   = kdv_dahil_birim_fiyat + birim_fiyat_otv_tutari;
        satir_tutar_bilgileri  += tr ( "ÖTV Dahil Birim Fiyat : " ) +  VIRGUL_EKLE( QVariant ( otv_dahil_birim_fiyat ).toString() ) + " " + m_temel_para_birimi_kodu + "\n" ;
    }

    if ( lineEdit_seri_no_list->text().isEmpty() EQ false ) {
        lineEdit_seri_no_list->text().prepend( "Seri Numaraları : \n" );
    }

    m_ui->textEdit_seri_no_bilgileri->setText( lineEdit_seri_no_list->text().replace( ",", "\n") );
    m_ui->textEdit_satir_tutar_bilgileri->setText ( satir_tutar_bilgileri );
}

/**************************************************************************************
                   STOK_FISI::SIRADAKI_IRSALIYE_SERI_NUMARASINI_SETLE
***************************************************************************************/

void STOK_FISI::SIRADAKI_IRSALIYE_SERI_NUMARASINI_SETLE ()
{    
    SUBE_UNITE_BILGILERI_STRUCT UNITE_BILG;
    SUBE_UNITE_BILGILERI_OKU( &UNITE_BILG, SUBE_GET_UNITE_ID() );

    m_siradaki_belge_no = UNITE_BILG.irsaliye_belge_numarasi;
    m_ui->lineEdit_belge_numarasi->setText( m_siradaki_belge_no );
}

/**************************************************************************************
                   STOK_FISI::LINE_CHANGER_BUTTON_CLICKED
***************************************************************************************/

int STOK_FISI::LINE_CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_row_number )
{
    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {
        if ( M_FIS->cari_hesap_id < 1 ) {
            MSG_WARNING(  tr("Önce cari hesabı seçmelisiniz"), NULL );
            return ADAK_RECORD_UNCHANGED;
        }
    }

    QLineEdit   * lineEdit_urun_id                      = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN                      );
    QSearchEdit * searchEdit_urun_kodu                  = ( QSearchEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_KOD_COLUMN                    );
    QLineEdit   * lineEdit_urun_adi                     = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_AD_COLUMN                     );
    QComboBox   * comboBox_stok_birimi                  = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN                  );
    QCommaEdit  * commaEdit_satir_tutari                = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN                 );
    QCommaEdit  * commaEdit_satir_stok_miktari          = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_MIKTARI_COLUMN                 );
    QCommaEdit  * commaEdit_isk_oncesi_birim_fiyat      = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN       );
    QCommaEdit  * commaEdit_isk_sonrasi_birim_fiyat     = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN      );
    QCommaEdit  * commaEdit_iskonto_yuzdesi             = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISKONTO_YUZDESI_COLUMN              );
    QCommaEdit  * commaEdit_iskonto_tutari              = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISKONTO_TUTARI_COLUMN               );
    QLineEdit   * lineEdit_stok_takip_sekli             = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_TAKIP_SEKLI_COLUMN             );
    QPushButton * pushButton_seri_no_parti_kodu         = ( QPushButton * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SERI_NO_PARTI_KODU_COLUMN          );
    QLineEdit   * lineEdit_para_birimi                  = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, PARA_BIRIMI_COLUMN                  );
    QCommaEdit  * commaEdit_vergi_haric_tutar           = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTARI_COLUMN            );
    QComboBox   * comboBox_kdv_orani                    = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_ORANI_COLUMN                    );
    QComboBox   * comboBox_kdv_dahil_mi                 = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN                 );
    QCommaEdit  * commaEdit_kdv_tutari                  = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_TUTAR_COLUMN                   );
    QCommaEdit  * commaEdit_kdv_dahil_tutar             = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_DAHIL_TUTARI_COLUMN              );
    QComboBox   * comboBox_otv_orani                    = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_ORANI_COLUMN                    );
    QCommaEdit  * commaEdit_otv_tutari                  = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_TUTARI_COLUMN                   );
    QCommaEdit  * commaEdit_otv_dahil_tutar             = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_DAHIL_TUTAR_COLUMN              );
    QCommaEdit  * commaEdit_temel_birim_katsayisi       = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, TEMEL_BIRIM_KATSAYISI               );
    QLineEdit   * lineEdit_fatura_satiri_id             = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, FATURA_SATIRI_ID_COLUMN             );
    QLineEdit   * lineEdit_iade_edilen_irs_id           = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, IADE_EDILEN_IRS_SATIR_ID_COLUMN     );
    QLineEdit   * lineEdit_parti_id                     = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, PARTI_ID_COLUMN                     );
    QPushButton * button_iade_irsaliye_satiri_secimi    = ( QPushButton * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, IADE_IRSALIYE_SATIRI_SECIMI_COLUMN  );
    QPushButton * button_fatura_satiri_secimi           = ( QPushButton * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, FATURA_SATIRI_SECIMI_COLUMN  );
    QLineEdit   * lineEdit_seri_no_list                 = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SERI_NO_LISTE_COLUMN );

    SQL_QUERY query ( DB );

    int fatura_satiri_id            = 0;
    int iade_edilen_irs_satiri_id   = 0;

    int         urun_id = 0;
    QString     urun_adi;
    double      satirdaki_urun_miktari  = 0.00;
    QString     urun_birimi;
    double      isk_sonrasi_urun_birim_fiyati = 0.00;
    double      isk_oncesi_urun_birim_fiyati  = 0.00;
    double      satir_tutari = 0.00;
    int         doviz_id = 0;
    double      vergi_haric_tutar = 0.00;
    QString     kdv_orani;
    double      kdv_tutari = 0.00;
    double      kdv_dahil_tutar = 0.00;
    QString     otv_orani;
    double      otv_tutari = 0.00;
    double      otv_dahil_tutar  = 0.00;
    int         kdv_dahil_mi = 0;
    double      temel_birim_katsayisi = 0.00;
    double      iskonto_yuzdesi = 0.00;
    double      iskonto_tutari  = 0.00;

    double      iade_edilen_miktar = 0.0;
    int         parti_id = 0;
    double      kalan_miktar = 0.0;
    double      irsaliyelenen_miktar = 0.0;

    bool urun_bilgileri_satirda_guncellensin_mi = false;

    lineEdit_para_birimi->setText( DVZ_GET_DOVIZ_KODU( M_FIS->stk_irs_para_birimi_id ) );

    if ( p_button EQ button_iade_irsaliye_satiri_secimi ) {
        if ( M_FIS->iade_edilen_irs_id < 1 ) {
            MSG_WARNING(  tr("İade edilecek irsaliye seçimi yapılmamış"), NULL );
            return ADAK_RECORD_UNCHANGED;
        }
        iade_edilen_irs_satiri_id = OPEN_IRS_IADE_SATIR_SECIMI ( M_FIS->iade_edilen_irs_id, this );
        if ( iade_edilen_irs_satiri_id < 1 ) {
            return ADAK_RECORD_UNCHANGED;
        }

        urun_bilgileri_satirda_guncellensin_mi = true;

        query.PREPARE_SELECT ( "stk_fis_satirlari",
                               "urun_id, urun_adi, faturalanan_miktar, satirdaki_urun_miktari,"
                               "urun_birimi, isk_oncesi_urun_birim_fiyati, isk_sonrasi_urun_birim_fiyati, "
                               "satir_tutari, doviz_id, kdv_orani, "
                               "kdv_dahil_mi, otv_orani, satir_vergi_haric_tutar, satir_otv_tutari, "
                               "satir_otv_dahil_tutar, satir_kdv_tutari, satir_kdv_dahil_tutar, "
                               "temel_birim_katsayisi, iade_edilen_miktar, parti_id, satir_iskonto_yuzdesi, "
                               "satir_iskonto_tutari  ",
                               "fis_satiri_id = :fis_satiri_id");

        query.SET_VALUE ( ":fis_satiri_id", iade_edilen_irs_satiri_id );

        if ( query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr("İade irsaliyesi satır bilgileri okunamadı"), NULL );
            return ADAK_RECORD_UNCHANGED;
        }

        query.NEXT();

        urun_id                         = query.VALUE("urun_id").toInt();
        urun_adi                        = query.VALUE("urun_adi").toString();
        satirdaki_urun_miktari          = query.VALUE("satirdaki_urun_miktari").toDouble();
        urun_birimi                     = query.VALUE("urun_birimi").toString();
        isk_sonrasi_urun_birim_fiyati   = query.VALUE("isk_sonrasi_urun_birim_fiyati").toDouble();
        isk_oncesi_urun_birim_fiyati    = query.VALUE("isk_oncesi_urun_birim_fiyati").toDouble();
        satir_tutari                    = query.VALUE("satir_tutari").toDouble();
        doviz_id                        = query.VALUE("doviz_id").toInt();
        vergi_haric_tutar               = query.VALUE("satir_vergi_haric_tutar").toDouble();
        kdv_orani                       = query.VALUE("kdv_orani").toString();
        kdv_tutari                      = query.VALUE("satir_kdv_tutari").toDouble();
        kdv_dahil_tutar                 = query.VALUE("satir_kdv_dahil_tutar").toDouble();
        otv_orani                       = query.VALUE("otv_orani").toString();
        otv_tutari                      = query.VALUE("satir_otv_tutari").toDouble();
        otv_dahil_tutar                 = query.VALUE("satir_otv_dahil_tutar").toDouble();
        kdv_dahil_mi                    = query.VALUE("kdv_dahil_mi").toInt();
        temel_birim_katsayisi           = query.VALUE("temel_birim_katsayisi").toDouble();
        iskonto_yuzdesi                 = query.VALUE("satir_iskonto_yuzdesi").toDouble();
        iskonto_tutari                  = query.VALUE("satir_iskonto_tutari").toDouble();

        iade_edilen_miktar              = 0.0;
        parti_id                        = 0;
        kalan_miktar                    = 0.0;
        irsaliyelenen_miktar            = 0.0;

        if ( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
            iade_edilen_miktar      = query.VALUE("iade_edilen_miktar").toDouble();
            parti_id                = query.VALUE("parti_id").toInt();
            kalan_miktar            = satirdaki_urun_miktari - iade_edilen_miktar;
        }
        else {
            irsaliyelenen_miktar    = query.VALUE("irsaliyelenen_miktar").toDouble();
            kalan_miktar            = satirdaki_urun_miktari - irsaliyelenen_miktar;
        }

        SET_FOCUS( commaEdit_satir_stok_miktari );
    }
    else if ( p_button EQ button_fatura_satiri_secimi ) {
        fatura_satiri_id = OPEN_IRS_FATURA_SATIR_SECIMI ( M_FIS->cari_hesap_id, M_FIS->fis_turu, this );
        if ( fatura_satiri_id < 1 ) {
            return ADAK_RECORD_UNCHANGED;
        }

        FATURA_FIS_SATIRI_STRUCT FAT_FIS_SATIRI;
        FATURA_CLEAR_FIS_SATIRI_STRUCT ( &FAT_FIS_SATIRI );

        int result = FATURA_SATIR_BILGILERINI_OKU( &FAT_FIS_SATIRI, fatura_satiri_id );

        if ( result EQ 0 ) {
            MSG_WARNING(  tr("Fatura satırı bilgileri okunamadı"), NULL );
            return ADAK_RECORD_UNCHANGED;
        }

        urun_bilgileri_satirda_guncellensin_mi = true;

        urun_id                         = FAT_FIS_SATIRI.urun_id;
        urun_adi                        = FAT_FIS_SATIRI.urun_adi;
        satirdaki_urun_miktari          = FAT_FIS_SATIRI.satirdaki_urun_miktari;
        urun_birimi                     = FAT_FIS_SATIRI.urun_birimi;
        isk_sonrasi_urun_birim_fiyati   = FAT_FIS_SATIRI.isk_sonrasi_urun_birim_fiyati;
        isk_oncesi_urun_birim_fiyati    = FAT_FIS_SATIRI.isk_oncesi_urun_birim_fiyati;
        satir_tutari                    = FAT_FIS_SATIRI.satir_tutari;
        doviz_id                        = FAT_FIS_SATIRI.doviz_id;
        vergi_haric_tutar               = FAT_FIS_SATIRI.satir_vergi_haric_tutar;//query.VALUE("satir_vergi_haric_tutar").toDouble();
        kdv_orani                       = FAT_FIS_SATIRI.kdv_orani;
        kdv_tutari                      = FAT_FIS_SATIRI.satir_kdv_tutari;      //query.VALUE("satir_kdv_tutari").toDouble();
        kdv_dahil_tutar                 = FAT_FIS_SATIRI.satir_kdv_dahil_tutar; //query.VALUE("satir_kdv_dahil_tutar").toDouble();
        otv_orani                       = FAT_FIS_SATIRI.otv_orani;
        otv_tutari                      = FAT_FIS_SATIRI.satir_otv_tutari;      //query.VALUE("satir_otv_tutari").toDouble();
        otv_dahil_tutar                 = FAT_FIS_SATIRI.satir_otv_dahil_tutar; //query.VALUE("satir_otv_dahil_tutar").toDouble();
        kdv_dahil_mi                    = FAT_FIS_SATIRI.kdv_dahil_mi;
        temel_birim_katsayisi           = FAT_FIS_SATIRI.temel_birim_katsayisi;
        iskonto_yuzdesi                 = FAT_FIS_SATIRI.satir_iskonto_yuzdesi; //query.VALUE("satir_iskonto_yuzdesi").toDouble();
        iskonto_tutari                  = FAT_FIS_SATIRI.satir_iskonto_tutari;  //query.VALUE("satir_iskonto_tutari").toDouble();

        iade_edilen_miktar              = 0.0;
        parti_id                        = 0;
        kalan_miktar                    = 0.0;
        irsaliyelenen_miktar            = 0.0;

        if ( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
            iade_edilen_miktar      = FAT_FIS_SATIRI.iade_edilen_miktar;
            parti_id                = FAT_FIS_SATIRI.parti_id;
            kalan_miktar            = satirdaki_urun_miktari - iade_edilen_miktar;
        }
        else {
            irsaliyelenen_miktar    = FAT_FIS_SATIRI.irsaliyelenen_miktar;
            kalan_miktar            = satirdaki_urun_miktari - irsaliyelenen_miktar;
        }
        SET_FOCUS( commaEdit_satir_stok_miktari );
    }
    else if( p_button EQ pushButton_seri_no_parti_kodu ) {

        urun_bilgileri_satirda_guncellensin_mi = false;

        int stok_takip_sekli = lineEdit_stok_takip_sekli->text().toInt();

        int urun_id          = lineEdit_urun_id->text().toInt();

        int irsaliye_stok_fisi_alis_satis_turu = M_FIS->irsaliye_alis_satis_turu;

        // IADE DE SECIM EKRANI GELMESI ICIN
        if ( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
            irsaliye_stok_fisi_alis_satis_turu = ENUM_SATIS_IRSALIYESI_FATURASI;
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
                switch ( irsaliye_stok_fisi_alis_satis_turu ) {
                    case ENUM_ALIS_IRSALIYESI_FATURASI : {
                        OPEN_STOK_GELEN_SERI_NO_BATCH( M_FIS->depo_id, urun_id, &seri_no_list, this );
                        }
                        break;

                    case ENUM_SATIS_IRSALIYESI_FATURASI :
                    default                                     : {
                        if( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
                            if( M_FIS->irsaliye_alis_satis_turu EQ ENUM_SATIS_IRSALIYESI_FATURASI ) {
                                STOK_GIDEN_SERI_NO_SEC( M_FIS->depo_id, urun_id, &seri_no_list, ENUM_URUN_DEPODA_DEGIL, true, -1, this );
                            }
                            else {
                                STOK_GIDEN_SERI_NO_SEC( M_FIS->depo_id, urun_id, &seri_no_list, ENUM_URUN_DEPODA, true, -1, this );
                            }
                        }
                        else {
                            STOK_GIDEN_SERI_NO_SEC( M_FIS->depo_id, urun_id, &seri_no_list, ENUM_URUN_DEPODA, false, -1, this );
                        }

                        break;
                    }

                }
                commaEdit_satir_stok_miktari->SET_DOUBLE( seri_no_list.size() );

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
                switch ( irsaliye_stok_fisi_alis_satis_turu ) {

                    case ENUM_ALIS_IRSALIYESI_FATURASI : {
                        int parti_kodu_id =  lineEdit_parti_id->text().toInt();
                        OPEN_STOK_URUN_PARTI_GIRISI_BATCH( &parti_kodu_id, urun_id, M_FIS->depo_id, this );
                        if ( parti_kodu_id NE -1 ) {
                            lineEdit_parti_id->setText( QString::number( parti_kodu_id ) );
                        }

                        lineEdit_seri_no_list->setText( STK_GET_PARTI_KODU( parti_kodu_id ) );
                        break;
                    }
                    case ENUM_SATIS_IRSALIYESI_FATURASI :
                    default                             : {
                        int parti_id = STOK_GIDEN_PARTI_NO_SEC( M_FIS->depo_id, urun_id, -1, this );
                        if ( parti_id < 1 ) {
                            return ADAK_RECORD_UNCHANGED;
                        }
                        lineEdit_seri_no_list->setText( STK_GET_PARTI_KODU( parti_id ) );
                        lineEdit_parti_id->setText( QString::number( parti_id ) );
                        break;
                    }
                }

                SET_FOCUS( commaEdit_satir_stok_miktari );
                break;
        }

        URUN_BIRIMI_FIYATLARINI_HESAPLA( p_row_number );

        IRSALIYE_SATIR_TUTARLARI_HESAPLA( p_row_number, p_button );

        return ADAK_RECORD_CHANGED;
    }

    if ( urun_bilgileri_satirda_guncellensin_mi EQ  true ) {
        satir_tutari        = ROUND ( ( satir_tutari      / satirdaki_urun_miktari ) * kalan_miktar );
        vergi_haric_tutar   = ROUND ( ( vergi_haric_tutar / satirdaki_urun_miktari ) * kalan_miktar );
        kdv_tutari          = ROUND ( ( kdv_tutari        / satirdaki_urun_miktari ) * kalan_miktar );
        kdv_dahil_tutar     = ROUND ( ( kdv_dahil_tutar   / satirdaki_urun_miktari ) * kalan_miktar );
        otv_tutari          = ROUND ( ( otv_tutari        / satirdaki_urun_miktari ) * kalan_miktar );
        otv_dahil_tutar     = ROUND ( ( otv_dahil_tutar   / satirdaki_urun_miktari ) * kalan_miktar );

        int         stok_takip_sekli        = -1;

        QString     doviz_kodu              = DVZ_GET_DOVIZ_KODU ( doviz_id );
        QString     urun_kodu;

        STK_FILL_URUN_BIRIMLERI_COMBOBOX ( urun_id, comboBox_stok_birimi, M_STOK_BIRIM_ADLARI );


        lineEdit_urun_adi->setReadOnly ( false );

        if ( urun_id > 0 ) {

            query.PREPARE_SELECT("stk_urunler","urun_kodu, urun_adi, stok_takip_sekli ",
                                 "urun_id = :urun_id");
            query.SET_VALUE(":urun_id", urun_id );
            if ( query.SELECT() > 0 ) {
                query.NEXT();
                urun_kodu        = query.VALUE(0).toString();
                urun_adi         = query.VALUE(1).toString();
                stok_takip_sekli = query.VALUE(2).toInt();
            }
        }

        lineEdit_urun_id->setText                       ( QVariant ( urun_id ).toString()                   );
        lineEdit_urun_adi->setText                      ( urun_adi                                          );
        searchEdit_urun_kodu->SET_TEXT                  ( urun_kodu                                         );
        commaEdit_satir_stok_miktari->SET_DOUBLE        ( kalan_miktar                                      );
        comboBox_stok_birimi->setEditText               ( urun_birimi                                       );
        commaEdit_temel_birim_katsayisi->SET_DOUBLE     ( temel_birim_katsayisi                             );
        commaEdit_isk_oncesi_birim_fiyat->SET_DOUBLE    ( isk_oncesi_urun_birim_fiyati                      );
        commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE   ( isk_sonrasi_urun_birim_fiyati                     );
        commaEdit_iskonto_yuzdesi->SET_DOUBLE           ( iskonto_yuzdesi                                   );
        commaEdit_iskonto_tutari->SET_DOUBLE            ( iskonto_tutari                                    );
        commaEdit_satir_tutari->SET_DOUBLE              ( satir_tutari                                      );
        lineEdit_stok_takip_sekli->setText              ( QVariant(stok_takip_sekli).toString()             );
        commaEdit_vergi_haric_tutar->SET_DOUBLE         ( vergi_haric_tutar                                 );
        commaEdit_kdv_tutari->SET_DOUBLE                ( kdv_tutari                                        );
        commaEdit_kdv_dahil_tutar->SET_DOUBLE           ( kdv_dahil_tutar                                   );
        commaEdit_otv_tutari->SET_DOUBLE                ( otv_tutari                                        );
        commaEdit_otv_dahil_tutar->SET_DOUBLE           ( otv_dahil_tutar                                   );
        comboBox_kdv_orani->setCurrentIndex             ( comboBox_kdv_orani->findText(kdv_orani)           );
        comboBox_otv_orani->setCurrentIndex             ( comboBox_otv_orani->findText(otv_orani)           );
        comboBox_kdv_dahil_mi->setCurrentIndex          ( kdv_dahil_mi                                      );
        lineEdit_fatura_satiri_id->setText              ( QVariant ( fatura_satiri_id ).toString()          );
        lineEdit_parti_id->setText                      ( QVariant ( parti_id ).toString()                  );
        lineEdit_iade_edilen_irs_id->setText            ( QVariant ( iade_edilen_irs_satiri_id ).toString() );

        QString seri_nolar ;
        if ( lineEdit_stok_takip_sekli->text().toInt() EQ ENUM_SERI_NO_ILE_TAKIP ) {
            if( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
                if ( M_FIS->irsaliye_alis_satis_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI ) {
                    seri_nolar = STK_GET_SERI_NO_BILGILERI( iade_edilen_irs_satiri_id, ENUM_URUN_DEPODA_DEGIL , -1 );
                }
                else {
                    // alistan aide ( satis islemi )
                    seri_nolar = STK_GET_SERI_NO_BILGILERI( iade_edilen_irs_satiri_id, ENUM_URUN_DEPODA, -1  );
                }
            }
            else {
                seri_nolar = STK_GET_SERI_NO_BILGILERI( iade_edilen_irs_satiri_id , ENUM_URUN_DEPODA, -1 );
            }
        }


        lineEdit_seri_no_list->setText( seri_nolar );

        URUN_BILGILERINI_AL                   ( p_row_number );
        TEMEL_BIRIM_CINSINDEN_KATSAYI_HESAPLA ( p_row_number );

        lineEdit_urun_adi->setEnabled       ( false );
        searchEdit_urun_kodu->setEnabled    ( false );
        comboBox_stok_birimi->setEnabled    ( false );

        switch ( stok_takip_sekli )  {

            case ENUM_BASIT_TAKIP       :
            case ENUM_SERI_NO_ILE_TAKIP :
                break;
            case ENUM_PARTI_TAKIBI      :
                if ( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
                    pushButton_seri_no_parti_kodu->setEnabled ( false );
                }
                lineEdit_seri_no_list->setText( STK_GET_PARTI_KODU(parti_id) );
                break;

            default                :
                break;
        }

        if ( isk_sonrasi_urun_birim_fiyati > 0 ) {
            commaEdit_isk_oncesi_birim_fiyat->setEnabled    ( false );
            commaEdit_isk_sonrasi_birim_fiyat->setEnabled   ( false );
            commaEdit_iskonto_yuzdesi->setEnabled           ( false );
            commaEdit_satir_tutari->setEnabled              ( false );
            commaEdit_vergi_haric_tutar->setEnabled         ( false );
            comboBox_kdv_orani->setEnabled                  ( false );
            comboBox_otv_orani->setEnabled                  ( false );
            comboBox_kdv_dahil_mi->setEnabled               ( false );
        }
        else {
            commaEdit_isk_oncesi_birim_fiyat->setEnabled    ( true );
            commaEdit_isk_sonrasi_birim_fiyat->setEnabled   ( true );
            commaEdit_iskonto_yuzdesi->setEnabled           ( true );
            commaEdit_satir_tutari->setEnabled              ( true );
            commaEdit_vergi_haric_tutar->setEnabled         ( true );
            comboBox_kdv_orani->setEnabled                  ( true );
            comboBox_otv_orani->setEnabled                  ( true );
            comboBox_kdv_dahil_mi->setEnabled               ( true );
        }
    }
    return ADAK_RECORD_CHANGED;

}

/**************************************************************************************
                   STOK_FISI::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int STOK_FISI::CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button )
{
    SQL_QUERY query ( DB );

    if ( p_button EQ m_ui->button_iade_edilecek_irsaliye_secimi ) {

        if ( M_FIS->cari_hesap_id < 1 ) {
            MSG_WARNING(  tr("Önce cari hesabı seçmelisiniz"), NULL );
            return ADAK_RECORD_UNCHANGED;
        }

        int irsaliye_stok_fisi_alis_satis_turu = M_FIS->irsaliye_alis_satis_turu;
        if( M_FIS->iade_irsaliyesi_mi EQ ENUM_IRS_IADE ) {
            irsaliye_stok_fisi_alis_satis_turu = !irsaliye_stok_fisi_alis_satis_turu;
        }

        int iade_edilen_irs_id  = OPEN_IRS_IADE_IRSALIYE_SECIMI ( m_ui->adakDate_fis_tarihi->DATE(), M_FIS->cari_hesap_id, irsaliye_stok_fisi_alis_satis_turu, M_FIS->fis_turu, this );
        if ( iade_edilen_irs_id > 0 ) {

            query.PREPARE_SELECT ( "stk_fisler","fis_no","fis_id = :fis_id");
            query.SET_VALUE      ( ":fis_id", iade_edilen_irs_id );

            if ( query.SELECT() EQ 0 ) {
                MSG_WARNING(  tr("İrsaliye bilgileri okunamadı."), NULL );
                return ADAK_RECORD_UNCHANGED;
            }

            query.NEXT();
            M_FIS->iade_edilen_irs_id = iade_edilen_irs_id;
            m_ui->commaEdit_iade_edilecek_fis_no->SET_INTEGER ( query.VALUE(0).toInt() );
            return ADAK_RECORD_CHANGED;
        }
    }

    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                   STOK_FISI::SAVER_BUTTON_CLICKED
***************************************************************************************/

void STOK_FISI::SAVER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_irsaliye_id )
{
    SQL_QUERY query      ( DB );

    if ( p_button EQ m_ui->button_fatura_secimi ) {

        int fatura_id = OPEN_IRS_FATURA_SECIMI ( M_FIS->cari_hesap_id, M_FIS->fis_turu, this );

        int order_number = 0;

        query.PREPARE_SELECT ( "stk_fis_satirlari","order_number",
                                     "fis_id = :fis_id" );
        query.SET_VALUE            ( ":fis_id", p_irsaliye_id );

        if ( query.SELECT( "order_number DESC", 0, 1 ) > 0 ) {

            query.NEXT();
            order_number = query.VALUE(0).toInt();
        }

        order_number += 1024;

        QList <int> fis_satiri_id_list = FAT_GET_FATURA_FIS_SATIRI_ID_LIST( fatura_id );

        STK_IRS_FIS_SATIR_STRUCT * IRSALIYE_SATIR_BILGILERI = new STK_IRS_FIS_SATIR_STRUCT;

        STK_IRS_CLEAR_SATIR_STRUCT( IRSALIYE_SATIR_BILGILERI );

        DB->START_TRANSACTION();

        for ( int i = 0; i < fis_satiri_id_list.size(); i++ ) {

            FATURA_FIS_SATIRI_STRUCT FAT_FIS_SATIRI;

            FATURA_CLEAR_FIS_SATIRI_STRUCT ( &FAT_FIS_SATIRI );

            FATURA_SATIR_BILGILERINI_OKU( &FAT_FIS_SATIRI, fis_satiri_id_list.at( i ) );

            int     urun_id                 = FAT_FIS_SATIRI.urun_id;               //query.VALUE("urun_id").toInt();
            double  satirdaki_urun_mikari   = FAT_FIS_SATIRI.satirdaki_urun_miktari;//query.VALUE("satirdaki_urun_miktari").toDouble();
            int     stok_takip_sekli        = -1;
            int     hatali_satir_mi         =  0;

            if ( urun_id < 1 AND satirdaki_urun_mikari EQ 0 ) {
                continue;
            }

            IRSALIYE_SATIR_BILGILERI->fatura_satiri_id                  = fis_satiri_id_list.at( i );
            IRSALIYE_SATIR_BILGILERI->urun_id                           = urun_id;
            IRSALIYE_SATIR_BILGILERI->urun_adi                          = FAT_FIS_SATIRI.urun_adi;
            IRSALIYE_SATIR_BILGILERI->satirdaki_urun_miktari            = satirdaki_urun_mikari;
            IRSALIYE_SATIR_BILGILERI->urun_birimi                       = FAT_FIS_SATIRI.urun_birimi;
            IRSALIYE_SATIR_BILGILERI->isk_oncesi_urun_birim_fiyati      = FAT_FIS_SATIRI.isk_oncesi_urun_birim_fiyati;
            IRSALIYE_SATIR_BILGILERI->isk_sonrasi_urun_birim_fiyati     = FAT_FIS_SATIRI.isk_sonrasi_urun_birim_fiyati;
            IRSALIYE_SATIR_BILGILERI->iskonto_yuzdesi                   = FAT_FIS_SATIRI.satir_iskonto_yuzdesi;         //query.VALUE("satir_iskonto_yuzdesi").toDouble();
            IRSALIYE_SATIR_BILGILERI->iskonto_tutari                    = FAT_FIS_SATIRI.satir_iskonto_tutari;          //query.VALUE("satir_iskonto_tutari").toDouble();
            IRSALIYE_SATIR_BILGILERI->satir_tutari                      = FAT_FIS_SATIRI.satir_tutari;                  //query.VALUE("satir_tutari").toDouble();
            IRSALIYE_SATIR_BILGILERI->doviz_id                          = FAT_FIS_SATIRI.doviz_id;
            IRSALIYE_SATIR_BILGILERI->doviz_kodu                        = DVZ_GET_DOVIZ_KODU( FAT_FIS_SATIRI.doviz_id );
            IRSALIYE_SATIR_BILGILERI->kdv_orani                         = FAT_FIS_SATIRI.kdv_orani;
            IRSALIYE_SATIR_BILGILERI->satir_vergi_haric_tutar           = FAT_FIS_SATIRI.satir_vergi_haric_tutar;
            IRSALIYE_SATIR_BILGILERI->satir_kdv_tutari                  = FAT_FIS_SATIRI.satir_kdv_tutari;
            IRSALIYE_SATIR_BILGILERI->satir_kdv_dahil_tutar             = FAT_FIS_SATIRI.satir_kdv_dahil_tutar;
            IRSALIYE_SATIR_BILGILERI->otv_orani                         = FAT_FIS_SATIRI.otv_orani;
            IRSALIYE_SATIR_BILGILERI->satir_otv_tutari                  = FAT_FIS_SATIRI.satir_otv_tutari;
            IRSALIYE_SATIR_BILGILERI->satir_otv_dahil_tutar             = FAT_FIS_SATIRI.satir_otv_dahil_tutar;
            IRSALIYE_SATIR_BILGILERI->kdv_dahil_mi                      = FAT_FIS_SATIRI.kdv_dahil_mi;
            IRSALIYE_SATIR_BILGILERI->temel_birim_katsayisi             = FAT_FIS_SATIRI.temel_birim_katsayisi;
            IRSALIYE_SATIR_BILGILERI->order_number                      = order_number;
            IRSALIYE_SATIR_BILGILERI->parti_id                          = 0;
            IRSALIYE_SATIR_BILGILERI->parti_kodu                        = "";
            IRSALIYE_SATIR_BILGILERI->row_number                        = 0;
            IRSALIYE_SATIR_BILGILERI->fis_id                            = p_irsaliye_id;
            IRSALIYE_SATIR_BILGILERI->modul_id                          = IRSALIYE_MODULU;
            IRSALIYE_SATIR_BILGILERI->program_id                        = E9_PROGRAMI;

            SQL_QUERY s_query( DB );
            s_query.PREPARE_SELECT ( "stk_urunler","stok_takip_sekli",
                                   "urun_id = :urun_id");

            s_query.SET_VALUE(":urun_id", urun_id );

            if ( s_query.SELECT() > 0 ) {
                s_query.NEXT();
                stok_takip_sekli = s_query.VALUE(0).toInt();
            }

            IRSALIYE_SATIR_BILGILERI->stok_takip_sekli = stok_takip_sekli;

            if ( urun_id > 0 ) {

                if ( satirdaki_urun_mikari EQ 0 ) {
                    hatali_satir_mi = 1;
                }
                else {
                    switch ( stok_takip_sekli ) {

                        case ENUM_PARTI_TAKIBI      :
                        case ENUM_SERI_NO_ILE_TAKIP :

                            hatali_satir_mi = 1;
                            break;

                        default :
                            break;

                    }
                }
            }

            IRSALIYE_SATIR_BILGILERI->hatali_satir_mi = hatali_satir_mi;

            STOK_FISINE_YENI_SATIR_KAYDET ( IRSALIYE_SATIR_BILGILERI );
        }

        query.PREPARE_UPDATE ( "stk_fisler","fis_id","irsaliye_faturalastirildi_mi ",
                               "fis_id = :fis_id");

        query.SET_VALUE      (":fis_id", p_irsaliye_id );
        query.SET_VALUE      (":irsaliye_faturalastirildi_mi", 1 );
        query.UPDATE         ();

        FAT_UPDATE_FATURA_IRSALIYELESTIRILDI_MI( fatura_id, 1 );

        query.PREPARE_INSERT ( "fat_irs_baglantisi","kayit_id","fatura_id, irsaliye_id");

        query.SET_VALUE      ( ":fatura_id"   , fatura_id     );
        query.SET_VALUE      ( ":irsaliye_id" , p_irsaliye_id );

        query.INSERT();

        DB->COMMIT_TRANSACTION();

        REFRESH_FIS_RECORD ();

        delete IRSALIYE_SATIR_BILGILERI;

    }
    else if ( p_button EQ m_ui->button_yazdir ) {

        QString irs_belge_seri, irs_belge_numarasi;

        if ( M_FIS->yazdirildi_mi NE 1 ) {

            SUBE_UNITE_BILGILERI_STRUCT UNITE_BILG;
            SUBE_UNITE_BILGILERI_OKU( &UNITE_BILG, SUBE_GET_UNITE_ID() );
            irs_belge_seri     = m_ui->lineEdit_irsaliye_belge_seri->text();
            irs_belge_numarasi = m_ui->lineEdit_belge_numarasi->text();

            if ( m_ui->lineEdit_irsaliye_belge_seri->text().isEmpty() EQ true OR m_ui->lineEdit_belge_numarasi->text().isEmpty() EQ true ) {
                if ( M_FIS->fis_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) {
                    irs_belge_seri      = UNITE_BILG.irsaliyeli_fat_belge_seri;
                    irs_belge_numarasi  = UNITE_BILG.irsaliyeli_fat_belge_numarasi;
                }
                else {
                    irs_belge_seri      = UNITE_BILG.irsaliye_belge_seri;
                    irs_belge_numarasi  = UNITE_BILG.irsaliye_belge_numarasi;
                }
            }

            QString msg_string;
            if( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {
                msg_string = "İrsaliye;";
            }
            else {
                msg_string = "Stok;";
            }

            int secim = MSG_YES_NO( tr  ("%1 \n"
                                        "Belge Seri          : %2 \n"
                                        "Belge Numarası : %3 \n"
                                        "ile yazdırılacaktır. Onaylıyormusunuz ?").arg(msg_string).arg( irs_belge_seri ).arg( irs_belge_numarasi ), m_ui->lineEdit_irsaliye_belge_seri );

            if ( secim EQ ADAK_NO ) {
                return;
            }
        }
        else {
            int secim = MSG_YES_NO( tr( "Bu İrsaliye daha önce yazdırıldı. Tekrar yazdırmak istiyormusunuz ?" ), NULL );

            if ( secim EQ ADAK_NO ) {
                return;
            }
        }
        // YAZDIRMA ISLEMI
        if( M_FIS->yazdirildi_mi NE 1 ) {
            m_ui->lineEdit_irsaliye_belge_seri->setText( irs_belge_seri );
            m_ui->lineEdit_belge_numarasi->setText( irs_belge_numarasi );
        }

        PRINT_IRS_IRSALIYE_BELGESI ( p_irsaliye_id );

        DB->START_TRANSACTION();

        query.PREPARE_UPDATE        ( "stk_fisler","fis_id","yazdirildi_mi","fis_id = :fis_id");
        query.SET_VALUE             ( ":fis_id"         , p_irsaliye_id );
        query.SET_VALUE             ( ":yazdirildi_mi"  , 1 );
        query.UPDATE                ();

        M_FIS->yazdirildi_mi = 1;

        DB->COMMIT_TRANSACTION();

    }
    else if ( p_button EQ m_ui->pushButton_detaylar ) {
        int old_muh_hesap_id    = M_STOK_DETAYLARI->muh_hesap_id;
        if ( OPEN_STOK_DETAYLAR_BATCH ( M_STOK_DETAYLARI , this) EQ true ) {

            DB->START_TRANSACTION();

            if ( M_STOK_DETAYLARI->kdv_muaf_mi EQ 1 OR M_STOK_DETAYLARI->otv_muaf_mi EQ 1) {
                //Kdv ve otvden muaf ise ,kdv ve otv tutarlari ve oranlari sifirlanir.

                SQL_QUERY update_query(DB);

                double brut_tutar                 = 0.0;
                double iskonto_yuzdesi            = 0;
                double isk_oncesi_otv_dahil_tutar = 0.0;
                double isk_oncesi_kdv_dahil_tutar = 0.0;

                query.PREPARE_SELECT("stk_fis_satirlari , stk_fisler ",
                                     "satirdaki_urun_miktari , isk_sonrasi_urun_birim_fiyati ,                    "
                                     "kdv_dahil_mi,doviz_id,fis_satiri_id,fis_tarihi , iskonto_yuzdesi,"
                                     "kdv_orani , otv_orani , satir_kdv_tutari , satir_otv_tutari",
                                     "stk_fisler.fis_id = :fis_id ANd stk_fisler.fis_id = stk_fis_satirlari.fis_id ");
                query.SET_VALUE(":fis_id" , p_irsaliye_id);

                if ( query.SELECT() NE 0 ) {

                    while ( query.NEXT() ) {

                        double satirdaki_urun_miktari        = query.VALUE(0).toDouble();
                        double isk_sonrasi_urun_birim_fiyati = query.VALUE(1).toDouble();
                        int kdv_dahil_mi                     = query.VALUE(2).toInt();
                        int doviz_id                         = query.VALUE(3).toInt();
                        int fis_satiri_id                    = query.VALUE(4).toInt();
                        QDate fis_tarihi                     = QDate::fromString(query.VALUE(5).toString() , "yyyy.MM.dd");
                        iskonto_yuzdesi                      = query.VALUE(6).toDouble();
                        double kdv_orani                     = query.VALUE("kdv_orani").toDouble();
                        double otv_orani                     = query.VALUE("otv_orani").toDouble();
                        double satir_kdv_tutari              = query.VALUE("satir_kdv_tutari").toDouble();
                        double satir_otv_tutari              = query.VALUE("satir_otv_tutari").toDouble();

                        double satir_tutari = satirdaki_urun_miktari * isk_sonrasi_urun_birim_fiyati;

                        if ( M_STOK_DETAYLARI->kdv_muaf_mi EQ 1 ) {
                            kdv_orani        = 0;
                            satir_kdv_tutari = 0;
                        }
                        if ( M_STOK_DETAYLARI->otv_muaf_mi EQ 1 ) {
                            otv_orani        = 0;
                            satir_otv_tutari = 0;
                        }

                        double otv_dahil_tutar   = 0.0;
                        double vergi_haric_tutar = 0.0;

                        if ( kdv_dahil_mi EQ 1 ) {
                            otv_dahil_tutar   = ROUND ( satir_tutari  );
                            vergi_haric_tutar = ROUND ( otv_dahil_tutar  );
                        }
                        else {
                            vergi_haric_tutar = satir_tutari;
                            otv_dahil_tutar   = ROUND ( vergi_haric_tutar  );
                        }

                        double temel_para_birimi_vergi_haric_tutar = DVZ_PARA_BIRIMLERINI_DONUSTUR ( doviz_id,
                                                                                                 vergi_haric_tutar,
                                                                                                 DVZ_GET_TEMEL_PARA_BIRIMI_ID(),
                                                                                                 fis_tarihi);

                        double temel_para_birimi_otv_dahil_tutar = ROUND ( temel_para_birimi_vergi_haric_tutar * ( 1 + ( otv_orani / 100 ) ) );

                        double temel_para_birimi_kdv_tutari      = ROUND ( temel_para_birimi_otv_dahil_tutar * ( kdv_orani / 100 ) );

                        double temel_para_birimi_kdv_dahil_tutar = ROUND ( temel_para_birimi_otv_dahil_tutar + temel_para_birimi_kdv_tutari );

                        update_query.PREPARE_UPDATE("stk_fis_satirlari","fis_satiri_id",
                                                    "kdv_orani ,"
                                                    "otv_orani , "
                                                    "satir_kdv_tutari ,"
                                                    "satir_kdv_dahil_tutar ,"
                                                    "satir_vergi_haric_tutar ,"
                                                    "satir_otv_tutari ,"
                                                    "satir_otv_dahil_tutar ,"
                                                    "satir_tutari ",
                                                    "fis_satiri_id = :fis_satiri_id");

                        update_query.SET_VALUE(":kdv_orani"                 , kdv_orani);
                        update_query.SET_VALUE(":otv_orani"                 , otv_orani);
                        update_query.SET_VALUE(":satir_kdv_tutari"          , satir_kdv_tutari);
                        update_query.SET_VALUE(":satir_kdv_dahil_tutar"     , ROUND ( temel_para_birimi_kdv_dahil_tutar));
                        update_query.SET_VALUE(":satir_vergi_haric_tutar"   , ROUND ( temel_para_birimi_vergi_haric_tutar ));
                        update_query.SET_VALUE(":satir_otv_dahil_tutar"     , ROUND ( temel_para_birimi_otv_dahil_tutar ));
                        update_query.SET_VALUE(":satir_otv_tutari"          , satir_otv_tutari);
                        update_query.SET_VALUE(":satir_tutari"              , ROUND ( satir_tutari ));
                        update_query.SET_VALUE(":fis_satiri_id"             , fis_satiri_id);

                        update_query.UPDATE();

                        brut_tutar                 += temel_para_birimi_vergi_haric_tutar;
                        isk_oncesi_otv_dahil_tutar += temel_para_birimi_otv_dahil_tutar;
                        isk_oncesi_kdv_dahil_tutar += temel_para_birimi_kdv_dahil_tutar;
                    }

                    double  isk_sonrasi_otv_dahil_tutar     = ROUND ( isk_oncesi_otv_dahil_tutar    * ( 1 - ( iskonto_yuzdesi / 100 ) ) );
                    double  ara_toplam                      = ROUND ( brut_tutar * ( 1 - ( iskonto_yuzdesi / 100 ) ) );
                    double irsaliye_tutari                  = ROUND ( isk_oncesi_kdv_dahil_tutar    * ( 1 - ( iskonto_yuzdesi / 100 ) ) );

                    query.PREPARE_SELECT("stk_fisler" ,"isk_oncesi_kdv_tutari,isk_oncesi_otv_tutari,"
                                         "isk_sonrasi_otv_tutari,isk_sonrasi_kdv_tutari",
                                         "fis_id = :fis_id");
                    query.SET_VALUE(":fis_id" , p_irsaliye_id);

                    double isk_oncesi_kdv_tutari  = 0;
                    double isk_oncesi_otv_tutari  = 0;
                    double isk_sonrasi_otv_tutari = 0;
                    double isk_sonrasi_kdv_tutari = 0;

                    if ( query.SELECT() NE 0 ) {

                        query.NEXT();

                        isk_oncesi_kdv_tutari = query.VALUE(0).toDouble();
                        isk_oncesi_otv_tutari = query.VALUE(1).toDouble();
                        isk_sonrasi_otv_tutari= query.VALUE(2).toDouble();
                        isk_sonrasi_kdv_tutari= query.VALUE(3).toDouble();
                    }

                    if ( M_STOK_DETAYLARI->kdv_muaf_mi EQ 1 ) {
                        isk_oncesi_kdv_tutari = 0;
                        isk_sonrasi_kdv_tutari= 0;
                    }
                    if ( M_STOK_DETAYLARI->otv_muaf_mi EQ 1 ) {
                        isk_oncesi_otv_tutari = 0;
                        isk_sonrasi_otv_tutari= 0;
                    }

                    update_query.PREPARE_UPDATE("stk_fisler","fis_id",
                                                "isk_oncesi_kdv_tutari          ,"
                                                "isk_oncesi_otv_tutari          ,"
                                                "brut_tutar                     ,"
                                                "isk_oncesi_kdv_dahil_tutar     ,"
                                                "isk_oncesi_otv_dahil_tutar     ,"
                                                "isk_sonrasi_otv_tutari         ,"
                                                "isk_sonrasi_kdv_tutari         ,"
                                                "isk_sonrasi_otv_dahil_tutar    ,"
                                                "irsaliye_tutari                ,"
                                                "ara_toplam                     ,"
                                                "kdv_oran_bilgileri             ,"
                                                "otv_oran_bilgileri              ",
                                                "fis_id = :fis_id");
                    update_query.SET_VALUE(":isk_oncesi_kdv_tutari"      , isk_oncesi_kdv_tutari);
                    update_query.SET_VALUE(":isk_oncesi_otv_tutari"      , isk_oncesi_otv_tutari);
                    update_query.SET_VALUE(":brut_tutar"                 , ROUND(brut_tutar ));
                    update_query.SET_VALUE(":isk_oncesi_kdv_dahil_tutar" , ROUND(isk_oncesi_kdv_dahil_tutar));
                    update_query.SET_VALUE(":isk_oncesi_otv_dahil_tutar" , ROUND(isk_oncesi_otv_dahil_tutar));
                    update_query.SET_VALUE(":isk_sonrasi_otv_tutari"     , isk_sonrasi_otv_tutari );
                    update_query.SET_VALUE(":isk_sonrasi_kdv_tutari"     , isk_sonrasi_kdv_tutari );
                    update_query.SET_VALUE(":isk_sonrasi_otv_dahil_tutar", ROUND(isk_sonrasi_otv_dahil_tutar));
                    update_query.SET_VALUE(":irsaliye_tutari"            , ROUND(irsaliye_tutari ));
                    update_query.SET_VALUE(":ara_toplam"                 , ROUND(ara_toplam ));
                    update_query.SET_VALUE(":kdv_oran_bilgileri"         , "");
                    update_query.SET_VALUE(":otv_oran_bilgileri"         , "");
                    update_query.SET_VALUE(":fis_id"                     , p_irsaliye_id);

                    update_query.UPDATE();

                }
            }
            if ( M_STOK_DETAYLARI->muh_hesap_id NE old_muh_hesap_id ) {

                int muh_fis_id      = 0;
                int fis_satiri_id   = 0;
                int urun_id         = 0;


                bool muh_fis_satirlari_silindi_mi    = false;
                query.PREPARE_SELECT("stk_fis_satirlari , stk_fisler",
                                     "fis_satiri_id , muh_fis_id, urun_id ",
                                     "stk_fisler.fis_id = :fis_id ANd stk_fisler.fis_id = stk_fis_satirlari.fis_id ");
                query.SET_VALUE(":fis_id" , p_irsaliye_id);

                if ( query.SELECT() NE 0 ) {

                    while ( query.NEXT() ) {
                        muh_fis_id      = query.VALUE( "muh_fis_id" ).toInt();
                        fis_satiri_id   = query.VALUE( "fis_satiri_id" ).toInt();
                        urun_id         = query.VALUE( "urun_id").toInt();

                        if ( M_FIS->fis_turu EQ ENUM_STK_SARF_FIRE_FISI ) {

                            if ( muh_fis_satirlari_silindi_mi EQ false ) {

                                // eski muhasebe fis satirlarini sil
                                MUH_FIS_STRUCT         * MUH_FIS          = new MUH_FIS_STRUCT;
                                MUH_CLEAR_FIS_STRUCT        ( MUH_FIS );

                                MUH_FIS_BILGILERINI_OKU ( MUH_FIS, muh_fis_id );

                                QStringList muh_row_list = MUH_GET_ENT_ROW_ID_LIST( E9_PROGRAMI, M_FIS->modul_id, fis_satiri_id );

                                for ( int i = 0; i < muh_row_list.count(); i++ ) {
                                    MUH_FIS_SATIRINI_SIL( MUH_FIS, QVariant ( muh_row_list.at( i ) ).toInt() );
                                }

                                muh_fis_satirlari_silindi_mi = true;

                                delete MUH_FIS;
                            }

                            STK_IRS_FIS_SATIR_STRUCT * STK_FIS_SATIRI = new STK_IRS_FIS_SATIR_STRUCT;

                            STK_FIS_SATIRI->base_record_id      = fis_satiri_id;
                            STK_FIS_SATIRI->muh_fis_id          = muh_fis_id;
                            STK_FIS_SATIRI->irsaliye_tutari     = m_ui->commaEdit_irsaliye_tutari->GET_DOUBLE();
                            STK_FIS_SATIRI->modul_id            = STOK_MODULU;
                            STK_FIS_SATIRI->program_id          = E9_PROGRAMI;
                            STK_FIS_SATIRI->urun_id             = urun_id;
                            STK_FIS_SATIRI->muh_hesap_id        = M_STOK_DETAYLARI->muh_hesap_id;

                            STK_ENT_FIS_SATIRI_EKLE( M_FIS, STK_FIS_SATIRI );

                            delete STK_FIS_SATIRI;
                        }
                    }

                }

            }


            query.PREPARE_UPDATE("stk_fisler","fis_id", "kdv_muaf_mi , "
                                 "muh_hesap_id , otv_muaf_mi",
                                 "fis_id = :fis_id");
            query.SET_VALUE(":kdv_muaf_mi"      , M_STOK_DETAYLARI->kdv_muaf_mi     );
            query.SET_VALUE(":muh_hesap_id"     , M_STOK_DETAYLARI->muh_hesap_id    );
            query.SET_VALUE(":otv_muaf_mi"      , M_STOK_DETAYLARI->otv_muaf_mi     );
            query.SET_VALUE(":fis_id"           , GET_RECORD_ID());

            query.UPDATE();

            DB->COMMIT_TRANSACTION();

            m_ui->textEdit_otv_yuzdeleri->clear();
            m_ui->textEdit_kdv_yuzdeleri->clear();

            REFRESH_FIS_RECORD();
        }
    }
    else if ( p_button EQ m_ui->pushButton_urettigi_fisler ) {
        E9_URETILEN_FIS_SEC( M_FIS->program_id, M_FIS->modul_id, p_irsaliye_id, this );
    }

}

/**************************************************************************************
                   STOK_FISI::LEAVE_RECORD
***************************************************************************************/

int STOK_FISI::LEAVE_RECORD ()
{
    int modul    = STOK_MODULU;
    int log_turu = LOG_STK_FISLER;

    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {
        modul = IRSALIYE_MODULU;
        log_turu = LOG_IRS_FISLER;
    }


    if ( m_log_kaydi_eklenecek_mi EQ true) {
        DB->START_TRANSACTION();

        if ( m_kayit_eklendi EQ true ) {
            E9_LOG_KAYDI_EKLE (modul , log_turu , LOG_ISLEM_ADD , m_log_detaylari);
        }
        if ( m_kayit_silindi EQ true ) {
            E9_LOG_KAYDI_EKLE (modul , log_turu , LOG_ISLEM_DELETE , m_log_detaylari);
        }
        if ( m_kayit_eklendi EQ false AND m_kayit_silindi EQ false ) {
            E9_LOG_KAYDI_EKLE (modul , log_turu , LOG_ISLEM_UPDATE , m_log_detaylari);
        }

        DB->COMMIT_TRANSACTION();
    }
    return ADAK_OK;
}


/**************************************************************************************
                   STOK_FISI::SECILI_FATURANIN_SATIRLARINI_IRSALIYEDEN_SIL
***************************************************************************************/

void STOK_FISI::SECILI_FATURANIN_SATIRLARINI_IRSALIYEDEN_SIL ( int p_current_row )
{
    int fatura_id   = m_ui->tableWidget_irsaliyenin_faturalari->item ( p_current_row, 0 )->text().toInt();
    int irsaliye_id = m_ui->tableWidget_irsaliyenin_faturalari->item ( p_current_row, 1 )->text().toInt();

    SQL_QUERY query ( DB );

    QList < int> irs_fat_irs_str_list = FAT_GET_FAT_IRS_STR_IRSALIYE_STR_ID_LIST( fatura_id );

    if ( irs_fat_irs_str_list.size() EQ 0 ) {
        return;
    }

    DB->START_TRANSACTION();

    for ( int i = 0; i < irs_fat_irs_str_list.size(); i++ ) {
        int irsaliye_satiri_id = irs_fat_irs_str_list.at( i );
        STOK_FIS_SATIRINI_SIL ( irsaliye_id, irsaliye_satiri_id );
    }

    FAT_UPDATE_FATURA_IRSALIYELESTIRILDI_MI(fatura_id, 0 );

    m_ui->tableWidget_irsaliyenin_faturalari->removeRow ( p_current_row );

    if ( m_ui->tableWidget_irsaliyenin_faturalari->rowCount() EQ 0 ) {

        query.PREPARE_UPDATE ("stk_fisler","fis_id","irsaliye_faturalastirildi_mi",
                              "fis_id = :fis_id");
        query.SET_VALUE      (":fis_id"                      , irsaliye_id );
        query.SET_VALUE      (":irsaliye_faturalastirildi_mi", 0 );
        query.UPDATE         ();
    }

    query.PREPARE_DELETE( "fat_irs_baglantisi ","fatura_id = :fatura_id");

    query.SET_VALUE      (":fatura_id" , fatura_id );

    query.DELETE();

    DB->COMMIT_TRANSACTION();

    REFRESH_FIS_RECORD ();
}


/**************************************************************************************
                   STOK_FISI::STOK_FISINE_YENI_SATIR_KAYDET
***************************************************************************************/

void STOK_FISI::STOK_FISINE_YENI_SATIR_KAYDET ( STK_IRS_FIS_SATIR_STRUCT * P_FIS_SATIRI )
{
    P_FIS_SATIRI->iade_edilen_miktar = 0;

    //! STOK IRS SATIR EKLEME
    int irsaliye_satiri_id = STK_IRS_FIS_SATIRI_EKLE( M_FIS, P_FIS_SATIRI );
    //! END


    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "fat_irs_satir_baglantisi", "fatura_satiri_id", "irsaliye_satiri_id = :irsaliye_satiri_id"  );
    query.SET_VALUE     ( ":irsaliye_satiri_id", irsaliye_satiri_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        int fatura_satiri_id = query.VALUE( 0 ).toInt();
        // BURAYI CAGIRAN FONK KULLANIYOR
        P_FIS_SATIRI->fatura_satiri_id  = fatura_satiri_id;
        FAT_FATURA_SATIRINDA_IRSALIYELENEN_MIKTARI_GUNCELLE( fatura_satiri_id, P_FIS_SATIRI->satirdaki_urun_miktari, ENUM_FAT_IRSALIYELENEN_MIKTAR_ARTIR );
    }


    if ( P_FIS_SATIRI->row_number > -1 ) {
        QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( irsaliye_satiri_id ).toString() );
        m_ui->tableWidget_fis_satirlari->setItem ( P_FIS_SATIRI->row_number, ROW_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem(QVariant ( P_FIS_SATIRI->fis_id ).toString());
        m_ui->tableWidget_fis_satirlari->setItem ( P_FIS_SATIRI->row_number, FIS_ID_COLUMN, new_item );
    }

    SATIR_DOVIZ_TUTARLARINI_GUNCELLE    ( P_FIS_SATIRI->doviz_kodu, P_FIS_SATIRI->satir_tutari, DOVIZ_EKLE );
    KDV_ORANLARI_BILGILERINI_GUNCELLE   ( QVariant ( P_FIS_SATIRI->kdv_orani).toString(), P_FIS_SATIRI->satir_kdv_tutari, VERGI_ORANI_EKLE );
    OTV_ORANLARI_BILGILERINI_GUNCELLE   ( QVariant ( P_FIS_SATIRI->otv_orani).toString(), P_FIS_SATIRI->satir_otv_tutari, VERGI_ORANI_EKLE );

    SATIR_DOVIZ_TUTARLARINI_YAZDIR      ();
    KDV_ORANLARI_BILGILERINI_YAZDIR     ();
    OTV_ORANLARI_BILGILERINI_YAZDIR     ();

    //! FIS SATIRININ HAREKETLERI OLUSTURULUYOR.
    STK_IRS_FIS_SATIR_HAREKETLERI_OLUSTUR( M_FIS, P_FIS_SATIRI, irsaliye_satiri_id );
    //! CONTROL END


    FIS_TOPLAMLARINI_HESAPLA();


    m_last_urun_id = -1;

    m_log_detaylari = STK_GET_STOK_FIS_TURU( M_FIS->fis_turu ) + "\n";
    m_log_kaydi_eklenecek_mi = true;

    QString gonderen_depo_kodu;
    QString gonderen_depo_adi;
    QString alan_depo_kodu;
    QString alan_depo_adi;

    SUBE_GET_DEPO_KODU_ADI ( M_FIS->depo_id      , &gonderen_depo_kodu , &gonderen_depo_adi);
    SUBE_GET_DEPO_KODU_ADI ( M_FIS->alan_depo_id , &alan_depo_kodu     , &alan_depo_adi);

    QString cari_hesap_kodu, cari_hesap_ismi;
    CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( M_FIS->cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );

    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {
        m_log_detaylari.append("İrsaliye Tarihi : " + QDate::fromString( M_FIS->fis_tarihi, "yyyy.MM.dd").toString("dd MMMM yyyy dddd")
                               + " , Sevk Tarihi : " + QDate::fromString( M_FIS->sevk_tarihi, "yyyy.MM.dd" ).toString("dd MMMM yyyy dddd")+
                               "\nFiş No : " + QVariant( M_FIS->fis_no).toString() + " , İrsaliye : " + M_FIS->belge_seri + " " + M_FIS->belge_numarasi );

        if ( M_FIS->fis_turu EQ ENUM_STK_DEPO_TRANSFER_CIKIS OR M_FIS->fis_turu EQ ENUM_STK_DEPO_TRANSFER_GIRIS ) {
            m_log_detaylari.append("\nGönderen Depo : " + gonderen_depo_kodu +
                                   "  " + gonderen_depo_adi + ",Alan Depo : " +
                                   alan_depo_kodu + "  " + alan_depo_adi);
        }
        else {
            m_log_detaylari.append(+"\nDepo Kodu / Adı :"
                                   + gonderen_depo_kodu + "  " + gonderen_depo_adi
                                   +",Cari Hesap Kodu / Adı : " + cari_hesap_kodu +
                                   "  " + cari_hesap_ismi);
        }

    }
    else {
        m_log_detaylari.append("Fiş Tarihi : " + QDate::fromString( M_FIS->fis_tarihi, "yyyy.MM.dd").toString("dd MMMM yyyy dddd")
                               + " , Fiş No : " + QVariant( M_FIS->fis_no).toString() + "Depo Kodu  / Adı :"
                               + gonderen_depo_kodu + "  " + gonderen_depo_adi);
    }

    m_log_detaylari.append ( "\nAra Toplam : "        + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_ara_toplam->GET_DOUBLE()                   ).toString(), 2 )
                           + " , Kdv Tutarı : "       + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_kdv_tutari->GET_DOUBLE()       ).toString(), 2 )
                           + " , İrsaliye Tutarı : "  + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_irsaliye_tutari->GET_DOUBLE()              ).toString(), 2 )
                           + "\nÖtv Tutarı : "        + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_tutari->GET_DOUBLE()       ).toString(), 2 )
                           + " , Ötv Dahil Toplam : " + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->GET_DOUBLE()  ).toString(), 2 ) );


}

/**************************************************************************************
                   STOK_FISI::STOK_FIS_SATIR_BILGILERINI_GUNCELLE
***************************************************************************************/

void STOK_FISI::STOK_FIS_SATIR_BILGILERINI_GUNCELLE ( int p_fis_id, int p_row_number )
{
    //SQL_QUERY f_query    ( DB );

    QLineEdit   * lineEdit_urun_id                            = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_ID_COLUMN                          );
    QCommaEdit  * commaEdit_stok_miktari                      = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_MIKTARI_COLUMN                     );
    QComboBox   * comboBox_stok_birimi                        = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN                      );
    QCommaEdit  * commaEdit_temel_birim_katsayisi             = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, TEMEL_BIRIM_KATSAYISI                   );
    QCommaEdit  * commaEdit_isk_oncesi_birim_fiyat            = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN           );
    QCommaEdit  * commaEdit_isk_sonrasi_birim_fiyat           = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN          );
    QCommaEdit  * commaEdit_iskonto_yuzdesi                   = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISKONTO_YUZDESI_COLUMN                  );
    QCommaEdit  * commaEdit_iskonto_tutari                    = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISKONTO_TUTARI_COLUMN                   );
    QCommaEdit  * commaEdit_satir_tutari                      = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN                     );
    QLineEdit   * lineEdit_para_birimi                        = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, PARA_BIRIMI_COLUMN                      );
    QCommaEdit  * commaEdit_vergi_haric_tutar                 = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTARI_COLUMN               );
    QLineEdit   * lineEdit_parti_id                           = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, PARTI_ID_COLUMN                         );
    QLineEdit   * lineEdit_stok_takip_sekli                   = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_TAKIP_SEKLI_COLUMN                 );
    QComboBox   * comboBox_kdv_orani                          = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_ORANI_COLUMN                        );
    QCommaEdit  * commaEdit_kdv_tutari                        = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_TUTAR_COLUMN                        );
    QCommaEdit  * commaEdit_kdv_dahil_tutar                   = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_DAHIL_TUTARI_COLUMN                 );
    QComboBox   * comboBox_kdv_dahil_mi                       = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN                     );
    QCommaEdit  * commaEdit_otv_tutari                        = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_TUTARI_COLUMN                       );
    QCommaEdit  * commaEdit_otv_dahil_tutar                   = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_DAHIL_TUTAR_COLUMN                  );
    QComboBox   * comboBox_otv_orani                          = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_ORANI_COLUMN                        );
    QLineEdit   * lineEdit_fatura_satiri_id                   = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, FATURA_SATIRI_ID_COLUMN                 );
    QLineEdit   * lineEdit_iade_edilen_irs_satir_id           = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, IADE_EDILEN_IRS_SATIR_ID_COLUMN         );
    QCommaEdit  * commaEdit_toplu_iskonto_yuzdesi             = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, TOPLU_ISKONTO_YUZDESI_COLUMN            );
    QCommaEdit  * commaEdit_t_is_sonrasi_birim_fiyat          = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_BIRIM_FIYAT_COLUMN         );
    QCommaEdit  * commaEdit_t_is_sonrasi_satir_tutari         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_SATIR_TUTARI_COLUMN        );
    QCommaEdit  * commaEdit_t_is_sonrasi_vergi_haric_tutar    = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN   );
    QCommaEdit  * commaEdit_t_is_sonrasi_otv_tutari           = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_OTV_TUTARI_COLUMN          );
    QCommaEdit  * commaEdit_t_is_sonrasi_otv_dahil_tutar      = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN     );
    QCommaEdit  * commaEdit_t_is_sonrasi_kdv_tutari           = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_KDV_TUTARI_COLUMN          );
    QCommaEdit  * commaEdit_t_is_sonrasi_kdv_dahil_tutar      = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN     );
    QLineEdit   * lineEdit_seri_no_list                       = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SERI_NO_LISTE_COLUMN                  );

    //! SATIRIN AIT OLDUGU FIS BILGILERI OKUNDU

    int  irsaliye_satiri_id                      = m_ui->tableWidget_fis_satirlari->item ( p_row_number, ROW_ID_COLUMN )->text().toInt();

    M_FIS_SATIRI->doviz_id                        = DVZ_GET_DOVIZ_ID( lineEdit_para_birimi->text() );
    M_FIS_SATIRI->kdv_orani                       = comboBox_kdv_orani->currentText().toDouble();
    M_FIS_SATIRI->otv_orani                       = comboBox_otv_orani->currentText().toDouble();
    M_FIS_SATIRI->satir_tutari                    = commaEdit_satir_tutari->GET_DOUBLE();
    M_FIS_SATIRI->fatura_satiri_id                = lineEdit_fatura_satiri_id->text().toInt();
    M_FIS_SATIRI->fis_id                          = p_fis_id;
    M_FIS_SATIRI->urun_id                         = lineEdit_urun_id->text().toInt();
    M_FIS_SATIRI->satirdaki_urun_miktari          = commaEdit_stok_miktari->GET_DOUBLE();
    M_FIS_SATIRI->temel_birim_katsayisi           = commaEdit_temel_birim_katsayisi->GET_DOUBLE();
    M_FIS_SATIRI->satir_vergi_haric_tutar         = commaEdit_vergi_haric_tutar->GET_DOUBLE();
    M_FIS_SATIRI->satir_kdv_tutari                = commaEdit_kdv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->satir_kdv_dahil_tutar           = commaEdit_kdv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->parti_id                        = lineEdit_parti_id->text().toInt();
    M_FIS_SATIRI->stok_takip_sekli                = lineEdit_stok_takip_sekli->text().toInt();
    M_FIS_SATIRI->satir_otv_tutari                = commaEdit_otv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->satir_otv_dahil_tutar           = commaEdit_otv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->hatali_satir_mi                 = 0;
    M_FIS_SATIRI->iade_edilen_irsaliye_satir_id   = lineEdit_iade_edilen_irs_satir_id->text().toInt();
    M_FIS_SATIRI->iskonto_yuzdesi                 = commaEdit_iskonto_yuzdesi->GET_DOUBLE();
    M_FIS_SATIRI->urun_birimi                     = comboBox_stok_birimi->currentText();
    M_FIS_SATIRI->parti_kodu                      = lineEdit_seri_no_list->text();
    M_FIS_SATIRI->kdv_dahil_mi                    = comboBox_kdv_dahil_mi->currentIndex();
    M_FIS_SATIRI->toplu_iskonto_yuzdesi           = commaEdit_toplu_iskonto_yuzdesi->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_birim_fiyat        = commaEdit_t_is_sonrasi_birim_fiyat->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_satir_tutari       = commaEdit_t_is_sonrasi_satir_tutari->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_vergi_haric_tutar  = commaEdit_t_is_sonrasi_vergi_haric_tutar->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_otv_tutari         = commaEdit_t_is_sonrasi_otv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_otv_dahil_tutar    = commaEdit_t_is_sonrasi_otv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_kdv_tutari         = commaEdit_t_is_sonrasi_kdv_tutari->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_kdv_dahil_tutar    = commaEdit_t_is_sonrasi_kdv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->isk_oncesi_urun_birim_fiyati    = commaEdit_isk_oncesi_birim_fiyat->GET_DOUBLE();
    M_FIS_SATIRI->isk_sonrasi_urun_birim_fiyati   = commaEdit_isk_sonrasi_birim_fiyat->GET_DOUBLE();
    M_FIS_SATIRI->iskonto_tutari                  = commaEdit_iskonto_tutari->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_birim_fiyat        = commaEdit_t_is_sonrasi_birim_fiyat->GET_DOUBLE();
    M_FIS_SATIRI->t_is_sonrasi_birim_fiyat        = commaEdit_t_is_sonrasi_birim_fiyat->GET_DOUBLE();

    if( lineEdit_seri_no_list->text().isEmpty() EQ false ) {
        M_FIS_SATIRI->seri_no_list               = lineEdit_seri_no_list->text().split(",");
    }

    if ( M_FIS_SATIRI->urun_id > 0 ) {
        if ( M_FIS_SATIRI->satirdaki_urun_miktari EQ 0 ) {
            M_FIS_SATIRI->hatali_satir_mi  = 1;
        }
    }

    STK_IRS_FIS_SATIRI_GUNCELLE( M_FIS, M_FIS_SATIRI, irsaliye_satiri_id );

    FIS_TOPLAMLARINI_HESAPLA();

    SATIR_DOVIZ_TUTARLARINI_GUNCELLE  ( lineEdit_para_birimi->text(), commaEdit_satir_tutari->GET_DOUBLE(), DOVIZ_EKLE );
    KDV_ORANLARI_BILGILERINI_GUNCELLE ( comboBox_kdv_orani->currentText(), commaEdit_kdv_tutari->GET_DOUBLE(), VERGI_ORANI_EKLE );
    OTV_ORANLARI_BILGILERINI_GUNCELLE ( comboBox_otv_orani->currentText(), commaEdit_otv_tutari->GET_DOUBLE(), VERGI_ORANI_EKLE );

    SATIR_DOVIZ_TUTARLARINI_YAZDIR    ();
    KDV_ORANLARI_BILGILERINI_YAZDIR   ();
    OTV_ORANLARI_BILGILERINI_YAZDIR   ();

    //! LOG EKLEME ALANI
    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = STK_GET_STOK_FIS_TURU(M_FIS->fis_turu) + "\n";

    QString gonderen_depo_kodu;
    QString gonderen_depo_adi;
    QString alan_depo_kodu;
    QString alan_depo_adi;

    SUBE_GET_DEPO_KODU_ADI ( M_FIS->depo_id      , &gonderen_depo_kodu , &gonderen_depo_adi);
    SUBE_GET_DEPO_KODU_ADI ( M_FIS->alan_depo_id , &alan_depo_kodu     , &alan_depo_adi);

    QString cari_hesap_kodu, cari_hesap_ismi;
    CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( M_FIS->cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );

    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {
        m_log_detaylari.append("İrsaliye Tarihi : " + QDate::fromString( M_FIS->fis_tarihi, "yyyy.MM.dd").toString("dd MMMM yyyy dddd")
                               + " , Sevk Tarihi : " + QDate::fromString( M_FIS->sevk_tarihi, "yyyy.MM.dd").toString("dd MMMM yyyy dddd")+
                               "\nFiş No : " + QVariant( M_FIS->fis_no ).toString() + " , İrsaliye : " + M_FIS->belge_seri + " " +  M_FIS->belge_numarasi );

        if ( M_FIS->fis_turu EQ ENUM_STK_DEPO_TRANSFER_CIKIS OR M_FIS->fis_turu EQ ENUM_STK_DEPO_TRANSFER_GIRIS ) {
            m_log_detaylari.append("\nGönderen Depo : " + gonderen_depo_kodu + "  " + gonderen_depo_adi + ",Alan Depo : " +
                                   alan_depo_kodu + "  " + alan_depo_adi);
        }
        else {
            m_log_detaylari.append(+"\nDepo Kodu / Adı :" + gonderen_depo_kodu + "  " + gonderen_depo_adi
                                   +",Cari Hesap Kodu / Adı : " + cari_hesap_kodu + "  " + cari_hesap_ismi);
        }

    }
    else {
        m_log_detaylari.append("Fiş Tarihi : " + QDate::fromString( M_FIS->fis_tarihi, "yyyy.MM.dd").toString("dd MMMM yyyy dddd")
                               + " , Fiş No : " + QVariant( M_FIS->fis_no).toString() + "Depo Kodu  / Adı :"
                               + gonderen_depo_kodu + "  " + gonderen_depo_adi);
    }

    m_log_detaylari.append ( "\nAra Toplam : "         + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_ara_toplam->GET_DOUBLE() ).toString(), 2 )
                            + " , Kdv Tutarı : "       + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_kdv_tutari->GET_DOUBLE() ).toString(), 2 )
                            + " , İrsaliye Tutarı : "  + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_irsaliye_tutari->GET_DOUBLE()).toString(), 2 )
                            + "\nÖtv Tutarı : "        + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_tutari->GET_DOUBLE()).toString(), 2 )
                            + " , Ötv Dahil Toplam : " + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->GET_DOUBLE() ).toString(), 2 ));

}

/**************************************************************************************
                   STOK_FISI::STOK_FIS_SATIRINI_SIL
***************************************************************************************/

void STOK_FISI::STOK_FIS_SATIRINI_SIL ( int p_irsaliye_id, int p_irsaliye_satiri_id )
{

    Q_UNUSED( p_irsaliye_id );

    STK_IRS_CLEAR_SATIR_STRUCT( M_FIS_SATIRI );

    STK_IRS_SATIR_BILGILERINI_OKU( p_irsaliye_satiri_id, M_FIS_SATIRI );

    STK_IRS_FIS_SATIRI_SIL(  M_FIS, M_FIS_SATIRI, p_irsaliye_satiri_id );


    FIS_TOPLAMLARINI_HESAPLA();


    SATIR_DOVIZ_TUTARLARINI_GUNCELLE    ( M_FIS_SATIRI->doviz_kodu , M_FIS_SATIRI->satir_tutari, DOVIZ_SIL );
    KDV_ORANLARI_BILGILERINI_GUNCELLE   ( QVariant(M_FIS_SATIRI->kdv_orani).toString(), M_FIS_SATIRI->satir_kdv_tutari, VERGI_ORANI_SIL );
    OTV_ORANLARI_BILGILERINI_GUNCELLE   ( QVariant(M_FIS_SATIRI->otv_orani).toString(), M_FIS_SATIRI->satir_otv_tutari, VERGI_ORANI_SIL );

    SATIR_DOVIZ_TUTARLARINI_YAZDIR      ();
    KDV_ORANLARI_BILGILERINI_YAZDIR     ();
    OTV_ORANLARI_BILGILERINI_YAZDIR     ();

    m_log_kaydi_eklenecek_mi    = true;
    m_log_detaylari             = STK_GET_STOK_FIS_TURU ( M_FIS->fis_turu ) + "\n";

    QString     gonderen_depo_kodu;
    QString     gonderen_depo_adi;
    QString     alan_depo_kodu;
    QString     alan_depo_adi;

    SUBE_GET_DEPO_KODU_ADI ( M_FIS->depo_id      , &gonderen_depo_kodu , &gonderen_depo_adi );
    SUBE_GET_DEPO_KODU_ADI ( M_FIS->alan_depo_id , &alan_depo_kodu     , &alan_depo_adi     );

    QString cari_hesap_kodu, cari_hesap_ismi;
    CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( M_FIS->cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );

    if ( M_FIS->irsaliye_mi_stok_fisi_mi EQ ENUM_IRSALIYE ) {
        m_log_detaylari.append ( "İrsaliye Tarihi : "     + QDate::fromString( M_FIS->fis_tarihi, "yyyy.MM.dd").toString("dd MMMM yyyy dddd")  +
                                 " , Sevk Tarihi  : "     + QDate::fromString( M_FIS->sevk_tarihi, "yyyy.MM.dd").toString("dd MMMM yyyy dddd") +
                                 "\nFiş No  : "     + QVariant ( M_FIS->fis_no ).toString() + " , İrsaliye : " + M_FIS->belge_seri + " " + M_FIS->belge_numarasi );

        if ( M_FIS->fis_turu EQ ENUM_STK_DEPO_TRANSFER_CIKIS OR M_FIS->fis_turu EQ ENUM_STK_DEPO_TRANSFER_GIRIS ) {
            m_log_detaylari.append ( "\nGönderen Depo : " + gonderen_depo_kodu  + "  " + gonderen_depo_adi + ",Alan Depo : " +
                                     alan_depo_kodu + "  " + alan_depo_adi );
        }
        else {
            m_log_detaylari.append ( + "\nDepo Kodu / Adı :" +  gonderen_depo_kodu + "  "   + gonderen_depo_adi
                                     +  ",Cari Hesap Kodu / Adı : " + cari_hesap_kodu + "  " + cari_hesap_ismi );
        }

    }
    else {
        m_log_detaylari.append ( "Fiş Tarihi : " + QDate::fromString( M_FIS->fis_tarihi, "yyyy.MM.dd").toString("dd MMMM yyyy dddd")
                                 + " , Fiş No : " + QVariant ( M_FIS->fis_no ).toString() + "Depo Kodu  / Adı :"
                                 + gonderen_depo_kodu + "  " + gonderen_depo_adi);
    }

    m_log_detaylari.append ( "\nAra Toplam : "         + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_ara_toplam->GET_DOUBLE()).toString(),2)
                            + " , Kdv Tutarı : "       + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_kdv_tutari->GET_DOUBLE() ).toString(),2)
                            + " , İrsaliye Tutarı : "  + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_irsaliye_tutari->GET_DOUBLE() ).toString(),2)
                            + "\nÖtv Tutarı : "        + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_tutari->GET_DOUBLE() ).toString(),2)
                            + " , Ötv Dahil Toplam : " + VIRGUL_EKLE ( QVariant ( m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->GET_DOUBLE() ).toString(),2));

}

/**************************************************************************************
                   STOK_FISI::STOK_FISINDE_DEGISIKLIK_YAPILABILIR_MI
***************************************************************************************/

bool STOK_FISI::STOK_FISINDE_DEGISIKLIK_YAPILABILIR_MI ( int p_fis_id, int p_delete_record_check )
{
    SQL_QUERY query ( DB );

    int     iade_edildi_mi                = 0;
    int     faturalanan_satir_iceriyor_mu = 0;

    query.PREPARE_SELECT ( "stk_fis_satirlari",
                           "fis_satiri_id ",
                           "fis_id = :fis_id AND iade_edilen_miktar > :iade_edilen_miktar ");

    query.SET_VALUE      ( ":iade_edilen_miktar", 0 );
    query.SET_VALUE      ( ":fis_id", p_fis_id );

    if ( query.SELECT() > 0 ) {
        iade_edildi_mi = 1;
    }

    if ( iade_edildi_mi EQ 1 ) {
        MSG_WARNING(  tr("Bu irsaliye iade içeriyor. Silinemez veya değiştirilemez."), NULL );
        return false;
    }

    query.PREPARE_SELECT ( "stk_fis_satirlari","fis_satiri_id ",
                           "fis_id = :fis_id AND faturalanan_miktar > :faturalanan_miktar ");

    query.SET_VALUE      ( ":fis_id"            , p_fis_id );
    query.SET_VALUE      ( ":faturalanan_miktar", 0 );

    if ( query.SELECT() > 0 ) {
        faturalanan_satir_iceriyor_mu = 1;
    }

    if ( faturalanan_satir_iceriyor_mu EQ 1 ) {
        MSG_WARNING(  tr("Bu irsaliye faturalanan satır içeriyor. Silinemez veya değiştirilemez."), NULL );
        return false;
    }

    if ( M_FIS->modul_id EQ FATURA_MODULU ) {
        MSG_WARNING(  tr("Bu irsaliye fatura modülünden oluşturulmuş, üzerinde herhangi bir değişiklik yapamazsınız."), NULL);
        return false;
    }

    if ( M_FIS->iptal_mi EQ 1 ) {
        MSG_WARNING(  tr("İptal edilmiş stok fişi/irsaliye üzerinde değişiklik yapamazsınız."), NULL );
        return false;
    }

    if ( p_delete_record_check EQ 0 ) {     // Check fis record delete icerisinden cagirilirsa hata dondurulmez. irsaliye iptale dusurulur.
        if ( M_FIS->yazdirildi_mi EQ 1 ) {
            MSG_WARNING(  tr("Bu irsaliye yazdırılmış. Üzerinde değişiklik yapamazsınız.") , NULL);
            return false;
        }
    }

    return true;
}

/**************************************************************************************
                   STOK_FISI::IADE_ISLEMI_YAPILMIS_SATIR_MI
***************************************************************************************/

bool STOK_FISI::IADE_ISLEMI_YAPILMIS_SATIR_MI ( int p_irsaliye_satiri_id )
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "stk_fis_satirlari", "iade_edilen_miktar",
                           "fis_satiri_id = :fis_satiri_id" );
    query.SET_VALUE      ( ":fis_satiri_id", p_irsaliye_satiri_id );

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr ( "İrsaliye satır bilgileri okunamadı" ), NULL );
        return false;
    }

    query.NEXT();
    if ( query.VALUE(0).toInt() > 0 ) {
        MSG_WARNING(  tr("Bu satırda iade işlemi gerçekleştirilmiş. Satır üzerinde değişiklik veya silme işlemi yapamazsınız."), NULL );
        return false;
    }
    return true;
}

/**************************************************************************************
                   STOK_FISI::IRSALIYE_ALIS_SATIS_TURUNU_BELIRLE
***************************************************************************************/

int STOK_FISI::IRSALIYE_ALIS_SATIS_TURUNU_BELIRLE ()
{
    return M_FIS->irsaliye_alis_satis_turu;
}

/**************************************************************************************
                   STOK_FISI::ISKONTO_ORANINA_GORE_TOPLAMLARI_HESAPLA
***************************************************************************************/

void STOK_FISI::ISKONTO_ORANINA_GORE_TOPLAMLARI_HESAPLA ()
{

    double  iskonto_yuzdesi              = M_FIS->iskonto_yuzdesi;

    double  ara_toplam                   = ROUND ( M_FIS->brut_tutar * ( 1.0 - ( iskonto_yuzdesi / 100.0 ) ) );
    double  isk_sonrasi_otv_tutari       = ROUND ( M_FIS->isk_oncesi_otv_tutari      * ( 1.0 - ( iskonto_yuzdesi / 100.0 ) ) );
    double  isk_sonrasi_otv_dahil_tutar  = ROUND ( M_FIS->isk_oncesi_otv_dahil_tutar * ( 1.0 - ( iskonto_yuzdesi / 100.0 ) ) );
    double  isk_sonrasi_kdv_tutari       = ROUND ( M_FIS->isk_oncesi_kdv_tutari      * ( 1.0 - ( iskonto_yuzdesi / 100.0 ) ) );
    double  irsaliye_tutari              = ROUND ( M_FIS->isk_oncesi_kdv_dahil_tutar * ( 1.0 - ( iskonto_yuzdesi / 100.0 ) ) );
    double  iskonto_tutari               = ROUND ( M_FIS->brut_tutar - ara_toplam                                           );

    m_ui->commaEdit_brut_tutar->SET_DOUBLE                  ( M_FIS->brut_tutar               );
    m_ui->commaEdit_ara_toplam->SET_DOUBLE                  ( ara_toplam                      );
    m_ui->commaEdit_isk_sonrasi_kdv_tutari->SET_DOUBLE      ( isk_sonrasi_kdv_tutari          );
    m_ui->commaEdit_irsaliye_tutari->SET_DOUBLE             ( irsaliye_tutari                 );
    m_ui->commaEdit_isk_sonrasi_otv_tutari->SET_DOUBLE      ( isk_sonrasi_otv_tutari          );
    m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->SET_DOUBLE ( isk_sonrasi_otv_dahil_tutar     );
    m_ui->commaEdit_iskonto_tutari->SET_DOUBLE              ( iskonto_tutari                  );

}

/*****************************************************************************************************
                 STOK_FISI::TOPLU_ISKONTO_SONRASI_SATIR_DEGERLERINI_HESAPLA
*******************************************************************************************************/

void STOK_FISI::TOPLU_ISKONTO_SONRASI_SATIR_DEGERLERINI_HESAPLA ( int p_row_number )
{
    QCommaEdit  *   commaEdit_iskonto_sonrasi_birim_fiyat     = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN        );
    QCommaEdit  *   commaEdit_satir_tutari                    = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN                   );
    QCommaEdit  *   commaEdit_vergi_haric_tutar               = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTARI_COLUMN              );
    QCommaEdit  *   commaEdit_otv_tutari                      = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_TUTARI_COLUMN                     );
    QCommaEdit  *   commaEdit_otv_dahil_tutar                 = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_DAHIL_TUTAR_COLUMN                );
    QCommaEdit  *   commaEdit_kdv_tutari                      = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_TUTAR_COLUMN                     );
    QCommaEdit  *   commaEdit_kdv_dahil_tutar                 = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_DAHIL_TUTARI_COLUMN                );

    QCommaEdit  *   commaEdit_toplu_iskonto_yuzdesi           = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, TOPLU_ISKONTO_YUZDESI_COLUMN          );
    QCommaEdit  *   commaEdit_t_is_sonrasi_birim_fiyat        = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_BIRIM_FIYAT_COLUMN       );
    QCommaEdit  *   commaEdit_t_is_sonrasi_satir_tutari       = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_SATIR_TUTARI_COLUMN      );
    QCommaEdit  *   commaEdit_t_is_sonrasi_vergi_haric_tutar  = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_VERGI_HARIC_TUTAR_COLUMN );
    QCommaEdit  *   commaEdit_t_is_sonrasi_otv_tutari         = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_OTV_TUTARI_COLUMN        );
    QCommaEdit  *   commaEdit_t_is_sonrasi_otv_dahil_tutar    = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_OTV_DAHIL_TUTAR_COLUMN   );
    QCommaEdit  *   commaEdit_t_is_sonrasi_kdv_tutari         = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_KDV_TUTARI_COLUMN        );
    QCommaEdit  *   commaEdit_t_is_sonrasi_kdv_dahil_tutar    = ( QCommaEdit * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, T_IS_SONRASI_KDV_DAHIL_TUTAR_COLUMN   );

    double          toplu_iskonto_yuzdesi                     = m_ui->commaEdit_iskonto_yuzdesi->GET_DOUBLE();

    commaEdit_toplu_iskonto_yuzdesi->SET_DOUBLE             ( toplu_iskonto_yuzdesi );
    commaEdit_t_is_sonrasi_birim_fiyat->SET_DOUBLE          ( commaEdit_iskonto_sonrasi_birim_fiyat->GET_DOUBLE() * ( 1 - ( toplu_iskonto_yuzdesi / 100 ) ) );
    commaEdit_t_is_sonrasi_satir_tutari->SET_DOUBLE         ( commaEdit_satir_tutari->GET_DOUBLE()                * ( 1 - ( toplu_iskonto_yuzdesi / 100 ) ) );
    commaEdit_t_is_sonrasi_vergi_haric_tutar->SET_DOUBLE    ( commaEdit_vergi_haric_tutar->GET_DOUBLE()           * ( 1 - ( toplu_iskonto_yuzdesi / 100 ) ) );
    commaEdit_t_is_sonrasi_otv_tutari->SET_DOUBLE           ( commaEdit_otv_tutari->GET_DOUBLE()                  * ( 1 - ( toplu_iskonto_yuzdesi / 100 ) ) );
    commaEdit_t_is_sonrasi_otv_dahil_tutar->SET_DOUBLE      ( commaEdit_otv_dahil_tutar->GET_DOUBLE()             * ( 1 - ( toplu_iskonto_yuzdesi / 100 ) ) );
    commaEdit_t_is_sonrasi_kdv_tutari->SET_DOUBLE           ( commaEdit_kdv_tutari->GET_DOUBLE()                  * ( 1 - ( toplu_iskonto_yuzdesi / 100 ) ) );
    commaEdit_t_is_sonrasi_kdv_dahil_tutar->SET_DOUBLE      ( commaEdit_kdv_dahil_tutar->GET_DOUBLE()             * ( 1 - ( toplu_iskonto_yuzdesi / 100 ) ) );

}

/*****************************************************************************************************
                 STOK_FISI::IRS_BAGLANTI_BILGISINI_GOSTER
*******************************************************************************************************/

void STOK_FISI::IRS_BAGLANTI_BILGISINI_GOSTER( int p_irs_turu, int p_fis_id )
{
    switch( p_irs_turu ) {
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_ALIS   :
        case ENUM_IRS_ELLE_FATURA_IRSALIYELESTIRME_SATIS  :
        case ENUM_IRS_NORMAL_GIRIS                        :
        case ENUM_IRS_NORMAL_CIKIS                        :
        case ENUM_IRS_KONSINYE_CIKIS                      :
        case ENUM_IRS_KONSINYE_GIRIS                      : {

            QList < int > fat_fis_id = FAT_GET_FAT_IRS_BAGLANTISI_FATURA_ID( p_fis_id );

            if (fat_fis_id.isEmpty() EQ false ) {

                int fatura_id = fat_fis_id.at(0);

                if ( fatura_id NE 0 ) {
                    m_ui->textEdit_fatura_bilgisi->setHidden(false);
                    m_ui->textEdit_fatura_bilgisi->setText( FAT_IRS_BAGLANTISI_BILGISI( -1, p_fis_id ) );
                }
            }
            break;
        }

        default :
            break;
    }
}

/*****************************************************************************************************
                 STOK_FISI::IRS_KUR_VE_PARITE_AYARLA
*******************************************************************************************************/

void STOK_FISI::IRS_KUR_VE_PARITE_AYARLA( bool p_degerleri_oto_ata )
{
    bool cari_fis_tutarini_goster =  true;

    m_ui->label_temel_para_birimi->setText( m_temel_para_birimi_kodu );

    //! ortak olan durumlar
    m_ui->label_stk_irs_para_birimi_1->setVisible( true );
    m_ui->label_stk_irs_para_birimi_1->setText( "Kur  : 1 "+ DVZ_GET_DOVIZ_KODU( M_FIS->stk_irs_para_birimi_id ) );
    m_ui->commaEdit_kur->setVisible( true );
    m_ui->label_temel_para_birimi->setVisible( true );
    m_ui->label_parite->setVisible( false );
    m_ui->commaEdit_parite->setVisible( false );
    m_ui->label_cari_para_birimi->setVisible( false );
    m_ui->label_stk_irs_para_birimi_2->setVisible( false );
    m_ui->commaEdit_parite->SET_DOUBLE( 1.000 );
    //! end ortak olan durum

    if ( M_FIS->cari_para_birim_id NE 0 ) {
        if ( M_FIS->cari_para_birim_id EQ m_temel_para_birimi_id ) {

            if ( M_FIS->cari_para_birim_id EQ M_FIS->stk_irs_para_birimi_id ) {
                //hepsi temel para birimi

                m_ui->label_stk_irs_para_birimi_1->setVisible( false );
                m_ui->commaEdit_kur->setVisible( false );
                m_ui->label_temel_para_birimi->setVisible( false );
                m_ui->label_parite->setVisible( false );
                m_ui->commaEdit_parite->setVisible( false );
                m_ui->label_cari_para_birimi->setVisible( false );
                m_ui->label_stk_irs_para_birimi_2->setVisible( false );
                m_ui->commaEdit_kur->SET_DOUBLE( 1.000 );
                m_ui->commaEdit_parite->SET_DOUBLE( 1.000 );

                cari_fis_tutarini_goster = false;
            }
            else {
                // Irsaliye para birimi farkli
                if( p_degerleri_oto_ata EQ true ) {
                    m_ui->commaEdit_kur->SET_DOUBLE( DVZ_PARA_BIRIMLERINI_DONUSTUR ( M_FIS->stk_irs_para_birimi_id, 1.00, m_temel_para_birimi_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ) );
                }
            }
        }
        else {
            if( M_FIS->cari_para_birim_id EQ M_FIS->stk_irs_para_birimi_id ) {
                // irsaliye ile cari ayni temelden farklilar
                if( p_degerleri_oto_ata EQ true ) {
                    m_ui->commaEdit_kur->SET_DOUBLE( DVZ_PARA_BIRIMLERINI_DONUSTUR ( M_FIS->cari_para_birim_id, 1.00, m_temel_para_birimi_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ) );
                }

                cari_fis_tutarini_goster = false;
            }
            else {

                if( M_FIS->stk_irs_para_birimi_id EQ m_temel_para_birimi_id ) {
                    // cari farkli digerleri ayni
                    if( p_degerleri_oto_ata EQ true ) {
                        m_ui->commaEdit_kur->SET_DOUBLE( DVZ_PARA_BIRIMLERINI_DONUSTUR ( m_temel_para_birimi_id, 1.00, M_FIS->cari_para_birim_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ) );
                    }

                    m_ui->label_temel_para_birimi->setVisible( true );
                    m_ui->label_temel_para_birimi->setText( M_FIS->cari_para_birim_kodu );
                }
                else {
                    // hepsi biribirinden farkli
                    if( p_degerleri_oto_ata EQ true ) {
                        m_ui->commaEdit_kur->SET_DOUBLE( DVZ_DOVIZ_KURUNU_AL( M_FIS->stk_irs_para_birimi_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ) );
                        m_ui->commaEdit_parite->SET_DOUBLE( DVZ_PARITEYI_AL( M_FIS->cari_para_birim_id, M_FIS->stk_irs_para_birimi_id, m_ui->adakDate_fis_tarihi->QDATE(), ENUM_SATIS ) );
                    }

                    m_ui->label_parite->setVisible( true );
                    m_ui->commaEdit_parite->setVisible( true );
                    m_ui->label_cari_para_birimi->setVisible( true );
                    m_ui->label_stk_irs_para_birimi_2->setText( "1 " + DVZ_GET_DOVIZ_KODU( M_FIS->stk_irs_para_birimi_id ) );
                    m_ui->label_cari_para_birimi->setText( M_FIS->cari_para_birim_kodu );
                    m_ui->label_stk_irs_para_birimi_2->setVisible( true );
                }
            }
        }
    }
    else {
        m_ui->label_stk_irs_para_birimi_1->setVisible( false );
        m_ui->commaEdit_kur->setVisible( false );
        m_ui->label_temel_para_birimi->setVisible( false );
        cari_fis_tutarini_goster = false;
    }

    if( cari_fis_tutarini_goster EQ true ) {
        m_ui->commaEdit_cari_para_birimi_tutari->setVisible( true );
        m_ui->label_cari_para_birimi_tutari->setVisible( true );
    }
    else {
        m_ui->commaEdit_cari_para_birimi_tutari->setVisible( false );
        m_ui->label_cari_para_birimi_tutari->setVisible( false );
    }
}

/*****************************************************************************************************
                 STOK_FISI::FIS_TOPLAMLARINI_HESAPLA
*******************************************************************************************************/

void STOK_FISI::FIS_TOPLAMLARINI_HESAPLA()
{

    // Fiste toplaminda iskonto yapildiginda kullaniliyor.
    // FIS TOPLAMLARI YAZDIRILIYOR
    m_ui->commaEdit_brut_tutar->SET_DOUBLE                  ( M_FIS->brut_tutar                  );
    m_ui->commaEdit_ara_toplam->SET_DOUBLE                  ( M_FIS->ara_toplam                  );
    m_ui->commaEdit_isk_sonrasi_otv_tutari->SET_DOUBLE      ( M_FIS->isk_sonrasi_otv_tutari      );
    m_ui->commaEdit_isk_sonrasi_otv_dahil_tutar->SET_DOUBLE ( M_FIS->isk_sonrasi_otv_dahil_tutar );
    m_ui->commaEdit_isk_sonrasi_kdv_tutari->SET_DOUBLE      ( M_FIS->isk_sonrasi_kdv_tutari      );
    m_ui->commaEdit_irsaliye_tutari->SET_DOUBLE             ( M_FIS->irsaliye_tutari             );
    m_ui->commaEdit_iskonto_yuzdesi->SET_DOUBLE             ( M_FIS->iskonto_yuzdesi             );
    m_ui->commaEdit_iskonto_tutari->SET_DOUBLE              ( M_FIS->iskonto_tutari              );

    double carpim_katsayisi = M_FIS->doviz_kuru;

    if( M_FIS->cari_para_birim_id NE m_temel_para_birimi_id AND M_FIS->stk_irs_para_birimi_id NE m_temel_para_birimi_id ) {
        if( M_FIS->cari_para_birim_id NE M_FIS->stk_irs_para_birimi_id ) {
            carpim_katsayisi = M_FIS->parite;
        }
        else {
            carpim_katsayisi = M_FIS->doviz_kuru;
        }
    }

    m_ui->commaEdit_cari_para_birimi_tutari->SET_DOUBLE ( ROUND( M_FIS->irsaliye_tutari * carpim_katsayisi ) );

}

/**************************************************************************************
                   STOK_FISI::CARI_BORC_BILGISINI_GOSTER
***************************************************************************************/

void STOK_FISI::CARI_BORC_BILGISINI_GOSTER(int p_cari_hesap_id)
{

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

    m_ui->lineEdit_cari_para_birimi->setText( M_FIS->cari_para_birim_kodu );
}

/**************************************************************************************
                   STOK_FISI::IRSALIYE_SATIR_TUTARLARI_HESAPLA
***************************************************************************************/

int  STOK_FISI::IRSALIYE_SATIR_TUTARLARI_HESAPLA(int p_row_number, QObject *p_object)
{
    QComboBox   * comboBox_stok_birimi              = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, URUN_BIRIMI_COLUMN                );
    QCommaEdit  * commaEdit_satir_tutari            = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SATIR_TUTARI_COLUMN               );
    QCommaEdit  * commaEdit_satir_stok_miktari      = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, STOK_MIKTARI_COLUMN               );
    QCommaEdit  * commaEdit_isk_oncesi_birim_fiyat  = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_ONCESI_BIRIM_FIYAT_COLUMN     );
    QCommaEdit  * commaEdit_isk_sonrasi_birim_fiyat = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISK_SONRASI_BIRIM_FIYAT_COLUMN    );
    QCommaEdit  * commaEdit_iskonto_yuzdesi         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISKONTO_YUZDESI_COLUMN            );
    QCommaEdit  * commaEdit_iskonto_tutari          = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, ISKONTO_TUTARI_COLUMN             );
    QPushButton * pushButton_seri_no_parti_kodu     = ( QPushButton * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, SERI_NO_PARTI_KODU_COLUMN        );
    //QLineEdit   * lineEdit_para_birimi              = ( QLineEdit   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, PARA_BIRIMI_COLUMN                );
    QCommaEdit  * commaEdit_vergi_haric_tutar       = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, VERGI_HARIC_TUTARI_COLUMN         );
    QComboBox   * comboBox_kdv_orani                = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_ORANI_COLUMN                  );
    QComboBox   * comboBox_kdv_dahil_mi             = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_DAHIL_MI_COLUMN               );
    QCommaEdit  * commaEdit_kdv_tutari              = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_TUTAR_COLUMN                  );
    QCommaEdit  * commaEdit_kdv_dahil_tutar         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, KDV_DAHIL_TUTARI_COLUMN           );
    QComboBox   * comboBox_otv_orani                = ( QComboBox   * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_ORANI_COLUMN                  );
    QCommaEdit  * commaEdit_otv_tutari              = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_TUTARI_COLUMN                 );
    QCommaEdit  * commaEdit_otv_dahil_tutar         = ( QCommaEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget ( p_row_number, OTV_DAHIL_TUTAR_COLUMN            );



    if ( ( p_object EQ commaEdit_satir_tutari )
           OR ( p_object EQ commaEdit_satir_stok_miktari )     OR ( p_object EQ commaEdit_isk_oncesi_birim_fiyat )
           OR ( p_object EQ commaEdit_isk_oncesi_birim_fiyat ) OR ( p_object EQ commaEdit_iskonto_yuzdesi )
           OR ( p_object EQ comboBox_stok_birimi )             OR ( p_object EQ comboBox_kdv_orani )
           OR ( p_object EQ comboBox_kdv_dahil_mi )            OR ( p_object EQ comboBox_otv_orani )
           OR ( p_object EQ pushButton_seri_no_parti_kodu ) ) {


        if ( p_object EQ commaEdit_iskonto_yuzdesi ) {
            if ( commaEdit_iskonto_yuzdesi->GET_DOUBLE() > 100 ) {
                MSG_WARNING(  tr("İskonto yüzdesi 100 den büyük olamaz."), commaEdit_iskonto_yuzdesi );

                return ADAK_FAIL_UNDO;
            }
        }

        URUN_BILGILERINI_AL                   ( p_row_number );
        TEMEL_BIRIM_CINSINDEN_KATSAYI_HESAPLA ( p_row_number );


        if ( p_object EQ comboBox_stok_birimi ) {
            URUN_BIRIMI_FIYATLARINI_HESAPLA ( p_row_number );
        }

        if ( p_object EQ commaEdit_isk_sonrasi_birim_fiyat ) {
            commaEdit_iskonto_tutari->SET_DOUBLE  ( commaEdit_isk_oncesi_birim_fiyat->GET_DOUBLE() - commaEdit_isk_sonrasi_birim_fiyat->GET_DOUBLE() );
            commaEdit_iskonto_yuzdesi->SET_DOUBLE ( ( ( commaEdit_isk_oncesi_birim_fiyat->GET_DOUBLE() - commaEdit_isk_sonrasi_birim_fiyat->GET_DOUBLE() ) / commaEdit_isk_oncesi_birim_fiyat->GET_DOUBLE() ) * 100 );
        }
        else if ( p_object EQ commaEdit_iskonto_yuzdesi OR p_object EQ commaEdit_isk_oncesi_birim_fiyat ) {
            double iskonto_yuzdesi = commaEdit_iskonto_yuzdesi->GET_DOUBLE();
            commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE ( commaEdit_isk_oncesi_birim_fiyat->GET_DOUBLE() * ( 1 - ( iskonto_yuzdesi / 100 ) ) );
            commaEdit_iskonto_tutari->SET_DOUBLE ( commaEdit_isk_oncesi_birim_fiyat->GET_DOUBLE() - commaEdit_isk_sonrasi_birim_fiyat->GET_DOUBLE() );
        }

        // ISKONTO SONRASI VE ONCESI TUTARLAR TEKRAR HESAPLANMIS OLABILIRLER

        double iskonto_sonrasi_birim_fiyat  = commaEdit_isk_sonrasi_birim_fiyat->GET_DOUBLE();
        double satir_stok_miktari           = commaEdit_satir_stok_miktari->GET_DOUBLE();
        double satir_tutari                 = commaEdit_satir_tutari->GET_DOUBLE();

        if ( p_object NE commaEdit_satir_tutari ) {
            satir_tutari = satir_stok_miktari * iskonto_sonrasi_birim_fiyat;
        }

        double kdv_orani          = comboBox_kdv_orani->currentText().toDouble();
        double otv_orani          = comboBox_otv_orani->currentText().toDouble();
        double kdv_tutari         = 0.00;
        double kdv_dahil_tutar    = 0.00;
        double vergi_haric_tutar  = 0.00;
        double otv_tutari         = 0.00;

        // Ayni hesaplama fatura fisinde var
        //! [1]
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
                commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE ( ROUND ( vergi_haric_tutar / satir_stok_miktari ) );
            }
            else {
                commaEdit_isk_sonrasi_birim_fiyat->SET_DOUBLE ( ROUND ( kdv_dahil_tutar / satir_stok_miktari ) );
            }
        }

        //! [/1]
        double temel_para_birimi_vergi_haric_tutar = DVZ_PARA_BIRIMLERINI_DONUSTUR ( M_FIS->stk_irs_para_birimi_id, vergi_haric_tutar,
                                                                                 m_temel_para_birimi_id, m_ui->adakDate_fis_tarihi->QDATE());

        double temel_para_birimi_otv_tutari      = ROUND ( temel_para_birimi_vergi_haric_tutar * ( otv_orani / 100 )  );
        double temel_para_birimi_otv_dahil_tutar = ROUND ( temel_para_birimi_vergi_haric_tutar * ( 1 + ( otv_orani / 100 ) ) );

        double temel_para_birimi_kdv_tutari      = ROUND ( temel_para_birimi_otv_dahil_tutar * ( kdv_orani / 100 ) );
        double temel_para_birimi_kdv_dahil_tutar = ROUND ( temel_para_birimi_otv_dahil_tutar + temel_para_birimi_kdv_tutari );

        commaEdit_vergi_haric_tutar->SET_DOUBLE  ( temel_para_birimi_vergi_haric_tutar    );
        commaEdit_otv_tutari->SET_DOUBLE         ( temel_para_birimi_otv_tutari           );
        commaEdit_otv_dahil_tutar->SET_DOUBLE    ( temel_para_birimi_otv_dahil_tutar      );
        commaEdit_kdv_tutari->SET_DOUBLE         ( temel_para_birimi_kdv_tutari           );
        commaEdit_kdv_dahil_tutar->SET_DOUBLE    ( temel_para_birimi_kdv_dahil_tutar      );

        TOPLU_ISKONTO_SONRASI_SATIR_DEGERLERINI_HESAPLA ( p_row_number );

    }
    return ADAK_OK;
}

/**************************************************************************************
                   STOK_FISI::SATIR_PARA_BIRIMINI_GUNCELLE
***************************************************************************************/

void STOK_FISI::SATIR_PARA_BIRIMINI_GUNCELLE()
{
    CLOSE_ALL_EVENTS();

    for ( int row = 0 ; row < m_ui->tableWidget_fis_satirlari->rowCount() ; row++) {

        QLineEdit  * lineedit_makbuz_para_birimi = ( QLineEdit  * ) m_ui->tableWidget_fis_satirlari->cellWidget( row, PARA_BIRIMI_COLUMN );

        lineedit_makbuz_para_birimi->setText( m_ui->comboBox_stk_irs_para_birimi_id->currentText() );

    }
    OPEN_ALL_EVENTS();
}


