#include "adak_sql.h"
#include "adak_utils.h"
#include "adak_std_utils.h"
#include "stok_urun_karti_formu_class.h"
#include "ui_stok_urun_karti_formu.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "muh_console_utils.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "stok_urun_hareketleri_arama_open.h"
#include "e9_log.h"
#include "adak_gruplar.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "fat_console_utils.h"
#include "sube_console_utils.h"
#include "muh_hesap_arama_open.h"
#include "stok_urun_arama_open.h"
#include "stok_urun_birimleri_batch_open.h"
#include "dvz_gui_utils.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "stok_enum.h"
#include "stok_console_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_STOK_URUN_KARTI_FORMU
***************************************************************************************/

void OPEN_STOK_URUN_KARTI_FORMU ( int p_urun_id, QWidget * p_parent )
{
    STOK_URUN_KARTI_FORMU * stok_karti = new STOK_URUN_KARTI_FORMU ( p_urun_id, p_parent );
    stok_karti->SHOW( FULL_SCREEN );
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::STOK_URUN_KARTI_FORMU
***************************************************************************************/

STOK_URUN_KARTI_FORMU::STOK_URUN_KARTI_FORMU ( int p_urun_id, QWidget * p_parent) : FORM_KERNEL( p_parent ), m_ui(new Ui::STOK_URUN_KARTI_FORMU)
{
    m_urun_id = p_urun_id;

    m_ui->setupUi       ( this);
    START_FORM_KERNEL   ( this, DB );
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::SETUP_FORM
***************************************************************************************/

void  STOK_URUN_KARTI_FORMU::SETUP_FORM()
{
    SET_HELP_PAGE        ("stok-islemleri_urun-karti");
    SET_PAGE_TITLE       ( tr ( "STK - ÜRÜN KARTI" ) );

    REGISTER_BUTTONS_WIDGET  ( m_ui->navigation_buttons_widget );

    SET_SETTING_NAME     ( "STOK_URUN_KARTI_FORMU" );

    m_muh_hesap_id = 0;

    DISABLE_CLEAR_ON_WIDGET(m_ui->searchEdit_muh_hesap_kodu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_muh_hesap_ismi);

    // [ 1 ] Hassasiyet setleniyor
    int stok_birim_fiyat_hassasiyeti = STK_GET_BIRIM_HASSASIYETI();

    m_ui->commaEdit_alis_fiyati->SET_PRECISION( stok_birim_fiyat_hassasiyeti );
    m_ui->commaEdit_satis_fiyati->SET_PRECISION( stok_birim_fiyat_hassasiyeti );
    // [/1]

    REGISTER_SAVER_BUTTON( m_ui->pushButton_urun_hareketleri );

    if ( E9_KULLANICI_URUN_HAREKETLERINI_GOREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_urun_hareketleri->setVisible( false );
    }

    QString urunler_hesap_kodu, urunler_hesap_adi;

    m_muh_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "urunler_ent_hesap_id", &urunler_hesap_kodu, &urunler_hesap_adi );

    m_ui->searchEdit_muh_hesap_kodu->SET_TEXT   ( urunler_hesap_kodu );
    m_ui->lineEdit_muh_hesap_ismi->setText      ( urunler_hesap_adi  );

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {
        m_ui->searchEdit_muh_hesap_kodu->setHidden(true);
        m_ui->lineEdit_muh_hesap_ismi->setHidden(true);
        m_ui->label_muhasebe_hesabi->setHidden(true);
    }

    m_ui->comboBox_stok_takip_sekli->addItems(QStringList()<<tr("BASİT TAKİP")<<tr("PARTİ KODU İLE TAKİP")<<tr("SERİ NO İLE TAKİP"));
    m_ui->comboBox_stok_takip_sekli->setCurrentIndex(0);
    m_stok_takip_sekli = STK_GET_STOK_TAKIP_SEKLI_ENUM ( m_ui->comboBox_stok_takip_sekli->currentText());

    m_temel_para_birimi_kodu = DVZ_GET_TEMEL_PARA_BIRIMI_KODU ( );


    DVZ_FILL_COMBOBOX             ( m_ui->comboBox_urun_alis_doviz );
    DVZ_FILL_COMBOBOX             ( m_ui->comboBox_urun_satis_doviz );

    E9_FILL_KDV_ORANLARI_COMBOBOX ( m_ui->comboBox_alis_kdv_orani );
    E9_FILL_OTV_ORANLARI_COMBOBOX ( m_ui->comboBox_alis_otv_orani );

    E9_FILL_KDV_ORANLARI_COMBOBOX ( m_ui->comboBox_satis_kdv_orani );
    E9_FILL_OTV_ORANLARI_COMBOBOX ( m_ui->comboBox_satis_otv_orani );

    m_ui->comboBox_urun_alis_doviz->setCurrentIndex  ( m_ui->comboBox_urun_alis_doviz->findText  ( m_temel_para_birimi_kodu  ) );
    m_ui->comboBox_urun_satis_doviz->setCurrentIndex ( m_ui->comboBox_urun_satis_doviz->findText ( m_temel_para_birimi_kodu ) );

    SET_WIDGET_ARRAYS();

    SET_FIRST_FOCUS_WIDGET   ( m_ui->lineEdit_urun_kodu );    
    REGISTER_SAVER_BUTTON    ( m_ui->button_diger_urun_birimleri );
    REGISTER_CHANGER_BUTTON  ( m_ui->pushButton_gruplar);
    REGISTER_CHANGER_BUTTON  ( m_ui->pushButton_fotograf_sec );

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH   ( 512 );
    m_ui->lineEdit_urun_kodu->setMaxLength                  ( 30  );
    m_ui->lineEdit_barkod_numarasi->setMaxLength            ( 50  );
    m_ui->lineEdit_urun_adi->setMaxLength                   ( 50  );
    SET_COMBOBOX_LENGTH ( m_ui->comboBox_temel_urun_birimi,    20 );

    AUTOSTART_FORM_KERNEL    ( m_urun_id );
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/
void  STOK_URUN_KARTI_FORMU::CLEAR_FORM_MEMBERS ()
{
    m_fotograf_path.clear();
    m_stok_takip_sekli              = -1;
    m_grup_idleri.clear();
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::NEW_RECORD
***************************************************************************************/

void  STOK_URUN_KARTI_FORMU::NEW_RECORD ()
{
    FILL_URUN_TEMEL_BIRIMLERI();

    m_ui->label_kritik_stok_sev_temel_birim->clear();
    m_ui->label_min_stok_sev_temel_birim->clear();

    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
        M_COMMAEDIT_DIGER_BIRIM_MIKTARLARI[i]->hide();
        M_COMMAEDIT_DIGER_BIRIM_MIKTARLARI[i]->clear();
        M_LABEL_DIGER_BIRIM_ADLARI[i]->hide();
        M_LABEL_DIGER_BIRIM_ADLARI[i]->clear();
    }

    m_ui->comboBox_urun_alis_doviz->setCurrentIndex  ( m_ui->comboBox_urun_alis_doviz->findText  ( m_temel_para_birimi_kodu ) );
    m_ui->comboBox_urun_satis_doviz->setCurrentIndex ( m_ui->comboBox_urun_satis_doviz->findText ( m_temel_para_birimi_kodu ) );

    m_ui->comboBox_stok_takip_sekli->setCurrentIndex(0);
    m_stok_takip_sekli = STK_GET_STOK_TAKIP_SEKLI_ENUM ( m_ui->comboBox_stok_takip_sekli->currentText());

    FOTOGRAF_CLEAR();
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::GET_RECORD
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::GET_RECORD ( int p_urun_id )
{
    FILL_URUN_TEMEL_BIRIMLERI();

    SQL_QUERY f_query           ( DB );

    f_query.PREPARE_SELECT("stk_urunler","urun_kodu, urun_adi, barkod_numarasi,"
                             "alis_fiyati, satis_fiyati, stok_takip_sekli, urun_temel_birimi, "
                             "min_stok_seviyesi, kritik_stok_seviyesi, aciklama, alis_kdv_orani, alis_otv_orani, "
                             "urun_alis_doviz_id, urun_satis_doviz_id, "
                             "toplam_stoga_giren_array, toplam_stoktan_cikan_array, muh_hesap_id ,"
                             "fotograf, satis_kdv_orani, satis_otv_orani ",
                             "urun_id = :urun_id");

    f_query.SET_VALUE(":urun_id", p_urun_id );
    if ( f_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }
    f_query.NEXT();

    m_ui->lineEdit_urun_kodu->setText       ( f_query.VALUE(0).toString());
    m_ui->lineEdit_urun_adi->setText        ( f_query.VALUE(1).toString());
    m_ui->lineEdit_barkod_numarasi->setText ( f_query.VALUE(2).toString());

    QStringList grup_adlari;
    QStringList grup_kodlari;

    m_ui->textEdit_gruplar->setText(GRP_KAYDIN_GRUPLARINI_BUL ( E9_PROGRAMI , STOK_MODULU , p_urun_id, &m_grup_idleri, &grup_adlari ,&grup_kodlari ));


    m_stok_takip_sekli = f_query.VALUE(6).toInt();

    m_ui->commaEdit_alis_fiyati->SET_DOUBLE                             ( f_query.VALUE(3).toDouble());
    m_ui->commaEdit_satis_fiyati->SET_DOUBLE                            ( f_query.VALUE(4).toDouble());
    m_ui->comboBox_stok_takip_sekli->setCurrentIndex                    ( m_ui->comboBox_stok_takip_sekli->findText(STK_GET_STOK_TAKIP_SEKLI  ( f_query.VALUE(5).toInt() )));
    FIND_AND_SET_COMBOBOX_TEXT                                          ( m_ui->comboBox_temel_urun_birimi, f_query.VALUE(6).toString() );
    m_ui->label_temel_birim->setText                                    ( m_ui->comboBox_temel_urun_birimi->currentText() );
    m_ui->label_min_stok_sev_temel_birim->setText                       ( m_ui->comboBox_temel_urun_birimi->currentText() );
    m_ui->label_kritik_stok_sev_temel_birim->setText                    ( m_ui->comboBox_temel_urun_birimi->currentText() );
    m_ui->commaEdit_minimum_stok_seviyesi->SET_DOUBLE                   ( f_query.VALUE(7).toDouble());
    m_ui->commaEdit_kritik_stok_seviyesi->SET_DOUBLE                    ( f_query.VALUE(8).toDouble());
    m_ui->limitedTextEdit_aciklama->setText                             ( f_query.VALUE(9).toString());

    m_ui->comboBox_alis_kdv_orani->setCurrentIndex                      ( m_ui->comboBox_alis_kdv_orani->findText(f_query.VALUE(10).toString()));
    m_ui->comboBox_alis_otv_orani->setCurrentIndex                       ( m_ui->comboBox_alis_otv_orani->findText(f_query.VALUE(11).toString()));

    m_ui->comboBox_satis_kdv_orani->setCurrentIndex                     ( m_ui->comboBox_satis_kdv_orani->findText(f_query.VALUE(18).toString()));
    m_ui->comboBox_satis_otv_orani->setCurrentIndex                     ( m_ui->comboBox_satis_otv_orani->findText(f_query.VALUE(19).toString()));

    QString alis_doviz_kodu  = DVZ_GET_DOVIZ_KODU ( f_query.VALUE(12).toInt());
    QString satis_doviz_kodu = DVZ_GET_DOVIZ_KODU ( f_query.VALUE(13).toInt());

    m_muh_hesap_id = f_query.VALUE(16).toInt();

    if ( m_muh_hesap_id > 0 ) {

        QString tam_hesap_kodu,hesap_ismi;

        MUH_GET_HESAP_KODU_HESAP_ISMI(m_muh_hesap_id , tam_hesap_kodu,hesap_ismi);


        m_ui->searchEdit_muh_hesap_kodu->SET_TEXT   ( tam_hesap_kodu );
        m_ui->lineEdit_muh_hesap_ismi->setText      ( hesap_ismi );

    }

    m_ui->comboBox_urun_alis_doviz->setCurrentIndex  ( m_ui->comboBox_urun_alis_doviz->findText(alis_doviz_kodu));
    m_ui->comboBox_urun_satis_doviz->setCurrentIndex ( m_ui->comboBox_urun_satis_doviz->findText(satis_doviz_kodu));

    QString stoga_giren_miktar_array        = f_query.VALUE("toplam_stoga_giren_array").toString();
    QString stoktan_cikan_miktar_array      = f_query.VALUE("toplam_stoktan_cikan_array").toString();

    double STOGA_GIREN_MIKTAR   [E9_ARRAY_SIZE];
    double STOKTAN_CIKAN_MIKTAR [E9_ARRAY_SIZE];

    double stok_miktari = 0.00;
    UNPACK_DOUBLE_ARRAY ( stoga_giren_miktar_array,   STOGA_GIREN_MIKTAR,   E9_ARRAY_SIZE );
    UNPACK_DOUBLE_ARRAY ( stoktan_cikan_miktar_array, STOKTAN_CIKAN_MIKTAR, E9_ARRAY_SIZE );

    stok_miktari    = STOGA_GIREN_MIKTAR[E9_SON_BAKIYE] - STOKTAN_CIKAN_MIKTAR[E9_SON_BAKIYE];


    m_fotograf_path = "";
    FOTOGRAF_CLEAR();
    if ( f_query.VALUE("fotograf").toString().isEmpty() EQ false) {
        int w = m_ui->pushButton_fotograf_sec->width();
        int h = m_ui->pushButton_fotograf_sec->height();

        m_ui->pushButton_fotograf_sec->setText       ( "" );
        m_ui->pushButton_fotograf_sec->setIcon       ( QIcon( ADAK_BYTE_ARRAY_2_PIXMAP( f_query.VALUE ( "fotograf" ).toByteArray()) ) );
        m_ui->pushButton_fotograf_sec->setIconSize   ( QSize(w-4,h-4) );
        m_ui->pushButton_fotograf_sec->setMinimumSize( QSize(w,h) );
        m_ui->pushButton_fotograf_sec->setMaximumSize( QSize(w,h) );
    }

    m_ui->commaEdit_temel_miktar->SET_DOUBLE ( ROUND ( stok_miktari ) );

    URUN_BIRIMLERINI_GORUNTULE   ( p_urun_id );
    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::CHECK_VAR
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::CHECK_VAR ( QObject * p_object )
{
    if ( p_object EQ m_ui->comboBox_temel_urun_birimi ) {
        m_ui->label_min_stok_sev_temel_birim->setText       ( m_ui->comboBox_temel_urun_birimi->currentText() );
        m_ui->label_kritik_stok_sev_temel_birim->setText    ( m_ui->comboBox_temel_urun_birimi->currentText() );
        m_ui->label_temel_birim->setText                    ( m_ui->comboBox_temel_urun_birimi->currentText() );
    }

    else if ( p_object EQ m_ui->searchEdit_muh_hesap_kodu ) {

        if ( m_ui->searchEdit_muh_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_muh_hesap_ismi->clear();
            m_muh_hesap_id = -1;
            return ADAK_OK;
        }

        MUH_HESAP_STRUCT * MUHASEBE_HESABI = new MUH_HESAP_STRUCT;

        MUH_CLEAR_HESAP_STRUCT(MUHASEBE_HESABI);

        if ( MUH_HESAPLAR_TABLOSUNU_OKU(MUHASEBE_HESABI , m_ui->searchEdit_muh_hesap_kodu->GET_TEXT()) EQ 0 ) {
            MSG_WARNING(  tr ( "Muhasebe hesabı bulunamadı." ), m_ui->searchEdit_muh_hesap_kodu );

            return ADAK_FAIL_UNDO;
        }



        if ( MUHASEBE_HESABI->tali_hesap EQ 0 ) {
            MSG_WARNING(  tr ( "Seçilen hesap tali hesap değil. Entegrasyon yapılamaz."), m_ui->searchEdit_muh_hesap_kodu );

            return ADAK_FAIL_UNDO;
        }

        m_ui->lineEdit_muh_hesap_ismi->setText ( MUHASEBE_HESABI->hesap_ismi);
        m_muh_hesap_id = MUHASEBE_HESABI->hesap_id;
    }
    else if ( p_object EQ m_ui->comboBox_stok_takip_sekli ) {

        m_stok_takip_sekli = STK_GET_STOK_TAKIP_SEKLI_ENUM ( m_ui->comboBox_stok_takip_sekli->currentText());

        if ( m_stok_takip_sekli EQ ENUM_SERI_NO_ILE_TAKIP ) {
            for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
                M_COMMAEDIT_DIGER_BIRIM_MIKTARLARI[i]->hide();
                M_LABEL_DIGER_BIRIM_ADLARI[i]->hide();
            }
        }
    }
    else if ( p_object EQ m_ui->lineEdit_urun_kodu OR p_object EQ m_ui->lineEdit_barkod_numarasi ) {
        if ( m_ui->lineEdit_barkod_numarasi->text().isEmpty() EQ true ) {
            return ADAK_OK;
        }
        if ( m_ui->lineEdit_urun_kodu->text() EQ m_ui->lineEdit_barkod_numarasi->text() ) {
            MSG_WARNING(  tr("Ürün kodu ile barkod numarası aynı olamaz."), m_ui->lineEdit_urun_kodu );

            return ADAK_FAIL_UNDO;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::CHECK_EMPTY
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::CHECK_EMPTY ()
{

    if ( m_ui->lineEdit_urun_kodu->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr("Stok Kodu boş bırakılamaz."), m_ui->lineEdit_urun_kodu );

        return ADAK_FAIL;
    }
    if ( m_ui->lineEdit_urun_adi->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr("Stok adı boş bırakılamaz."), m_ui->lineEdit_urun_adi );

        return ADAK_FAIL;
    }
    if ( m_ui->comboBox_temel_urun_birimi->currentText().isEmpty() EQ true ) {
        MSG_WARNING(  tr("Temel ürün birimi boş bırakılmaz."), m_ui->comboBox_temel_urun_birimi );

        return ADAK_FAIL;
    }
    if ( m_ui->comboBox_stok_takip_sekli->currentIndex() EQ -1 OR m_stok_takip_sekli EQ -1 ) {
        MSG_WARNING(  tr("Stok takip şekli seçmelisiniz."), m_ui->comboBox_stok_takip_sekli );

        return ADAK_FAIL;
    }
    if ( m_muh_hesap_id < 1 ) {
        MSG_WARNING(  tr ( "Ürünün entegre olacağı muhasebe hesabını boş bırakamazsınız." ), m_ui->searchEdit_muh_hesap_kodu );

        return ADAK_FAIL;
    }
    if ( m_ui->searchEdit_muh_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_INFO( tr( "Muhasebe hesap kodu boş bırakılamaz." ), m_ui->searchEdit_muh_hesap_kodu );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::CHECK_ADD
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::CHECK_ADD ()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("stk_urunler","urun_id, urun_kodu, barkod_numarasi ",
                             "urun_kodu         = :urun_kodu_1       "
                             "OR    urun_kodu         = :urun_kodu_2       "
                             "OR    barkod_numarasi   = :barkod_numarasi_1 "
                             "OR    barkod_numarasi   = :barkod_numarasi_2 ");

    sql_query.SET_VALUE ( ":urun_kodu_1",       m_ui->lineEdit_urun_kodu->text()       );
    sql_query.SET_VALUE ( ":urun_kodu_2",       m_ui->lineEdit_barkod_numarasi->text() );
    sql_query.SET_VALUE ( ":barkod_numarasi_1", m_ui->lineEdit_urun_kodu->text()       );
    sql_query.SET_VALUE ( ":barkod_numarasi_2", m_ui->lineEdit_barkod_numarasi->text() );

    if ( sql_query.SELECT() > 0 ) {
        while ( sql_query.NEXT() EQ true ) {
            if ( sql_query.VALUE(1).toString() EQ m_ui->lineEdit_urun_kodu->text() ) {
                if ( m_ui->lineEdit_urun_kodu->text().isEmpty() EQ false ) {
                    MSG_WARNING(  tr("Bu ürün kodu ile bir hizmet veya ürün var. Ürün kodunu değiştiriniz."), m_ui->lineEdit_urun_kodu );

                    return ADAK_FAIL;
                }

            }
            else if ( sql_query.VALUE(2).toString() EQ m_ui->lineEdit_urun_kodu->text() ) {
                if ( m_ui->lineEdit_urun_kodu->text().isEmpty() EQ false ) {
                    MSG_WARNING(  tr ( "Bu ürün kodu ile başka bir ürün veya hizmet barkod numarası kayıtlı. Ürün kodunu değiştiriniz." ) , m_ui->lineEdit_urun_kodu );

                    return ADAK_FAIL;
                }
            }
            else if ( sql_query.VALUE(1).toString() EQ m_ui->lineEdit_barkod_numarasi->text() ) {
                if ( m_ui->lineEdit_barkod_numarasi->text().isEmpty() EQ false ) {
                   MSG_WARNING(  tr ( "Bu barkod numarası ile başka bir ürün veya hizmet kodu kayıtlı. Barkod numarasını değiştiriniz." ), m_ui->lineEdit_barkod_numarasi );

                   return ADAK_FAIL;
                }
            }
            else {
                if ( m_ui->lineEdit_barkod_numarasi->text().isEmpty() EQ false ) {
                    MSG_WARNING(  tr ( "Bu barkod numarası başka bir üründe vyea hizmette barkod numarası olarak kayıtlı. Barkod numarasını değiştiriniz." ), m_ui->lineEdit_barkod_numarasi );

                    return ADAK_FAIL;
                }
            }
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::ADD_RECORD
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::ADD_RECORD ()
{

    QByteArray fotograf;
    if ( m_fotograf_path.isEmpty() EQ false ) {
        fotograf = ADAK_PIXMAP_2_BYTE_ARRAY( m_fotograf_path );
    }
    else {
        fotograf = QByteArray();
    }

    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_INSERT("stk_urunler","urun_id", "urun_kodu, urun_adi, barkod_numarasi, "
                             "alis_fiyati, satis_fiyati, urun_alis_doviz_id, urun_satis_doviz_id, "
                             "stok_takip_sekli, "
                             "urun_temel_birimi, min_stok_seviyesi, kritik_stok_seviyesi, aciklama, "
                             "alis_otv_orani, alis_kdv_orani, hizmet_mi, muh_hesap_id, fotograf, satis_otv_orani, satis_kdv_orani");

    sql_query.SET_VALUE ( ":urun_kodu"                      , m_ui->lineEdit_urun_kodu->text()                                );
    sql_query.SET_VALUE ( ":urun_adi"                       , m_ui->lineEdit_urun_adi->text()                                 );
    sql_query.SET_VALUE ( ":barkod_numarasi"                , m_ui->lineEdit_barkod_numarasi->text()                          );
    sql_query.SET_VALUE ( ":alis_fiyati"                    , m_ui->commaEdit_alis_fiyati->GET_DOUBLE()                       );
    sql_query.SET_VALUE ( ":satis_fiyati"                   , m_ui->commaEdit_satis_fiyati->GET_DOUBLE()                      );
    sql_query.SET_VALUE ( ":urun_alis_doviz_id"             , DVZ_GET_DOVIZ_ID ( m_ui->comboBox_urun_alis_doviz->currentText())   );
    sql_query.SET_VALUE ( ":urun_satis_doviz_id"            , DVZ_GET_DOVIZ_ID ( m_ui->comboBox_urun_satis_doviz->currentText())  );
    sql_query.SET_VALUE ( ":stok_takip_sekli"               , m_stok_takip_sekli                                              );
    sql_query.SET_VALUE ( ":urun_temel_birimi"              , m_ui->comboBox_temel_urun_birimi->currentText()                 );
    sql_query.SET_VALUE ( ":min_stok_seviyesi"              , m_ui->commaEdit_minimum_stok_seviyesi->GET_DOUBLE()             );
    sql_query.SET_VALUE ( ":kritik_stok_seviyesi"           , m_ui->commaEdit_kritik_stok_seviyesi->GET_DOUBLE()              );
    sql_query.SET_VALUE ( ":aciklama"                       , m_ui->limitedTextEdit_aciklama->toPlainText()                   );
    sql_query.SET_VALUE ( ":alis_otv_orani"                 , m_ui->comboBox_alis_otv_orani->currentText().toDouble()         );
    sql_query.SET_VALUE ( ":alis_kdv_orani"                 , m_ui->comboBox_alis_kdv_orani->currentText().toDouble()         );
    sql_query.SET_VALUE ( ":satis_otv_orani"                , m_ui->comboBox_satis_otv_orani->currentText().toDouble()        );
    sql_query.SET_VALUE ( ":satis_kdv_orani"                , m_ui->comboBox_satis_kdv_orani->currentText().toDouble()        );
    sql_query.SET_VALUE ( ":hizmet_mi"                      , 0              );
    sql_query.SET_VALUE ( ":muh_hesap_id"                   , m_muh_hesap_id );
    sql_query.SET_VALUE ( ":fotograf"                       , fotograf );

    int urun_id = sql_query.INSERT();

    // DEPO ARRAYLARI OLUSTURULUYOR

    // OLUSTURAMAZ CUNKU DEPO ID GEREKIYOR. BUTUN DEPOLARADA DA URUN EKLEMEK DOGRU DEGIL

    GRP_KAYDI_GRUPLARA_EKLE(E9_PROGRAMI , STOK_MODULU , urun_id, &m_grup_idleri);

    QString log_detaylari = "Ürün Kodu / Adı : " + m_ui->lineEdit_urun_kodu->text() + "  " +
                            m_ui->lineEdit_urun_adi->text();
    E9_LOG_KAYDI_EKLE(STOK_MODULU , LOG_STK_URUNLER , LOG_ISLEM_ADD , log_detaylari);

    return urun_id;
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::CHECK_UPDATE
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::CHECK_UPDATE ( int p_urun_id )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("stk_urunler","urun_id, urun_kodu, barkod_numarasi ",
                             "urun_kodu         = :urun_kodu_1       "
                             "OR    urun_kodu         = :urun_kodu_2       "
                             "OR    barkod_numarasi   = :barkod_numarasi_1 "
                             "OR    barkod_numarasi   = :barkod_numarasi_2 ");

    sql_query.SET_VALUE ( ":urun_kodu_1",       m_ui->lineEdit_urun_kodu->text()       );
    sql_query.SET_VALUE ( ":urun_kodu_2",       m_ui->lineEdit_barkod_numarasi->text() );
    sql_query.SET_VALUE ( ":barkod_numarasi_1", m_ui->lineEdit_urun_kodu->text()       );
    sql_query.SET_VALUE ( ":barkod_numarasi_2", m_ui->lineEdit_barkod_numarasi->text() );

    if ( sql_query.SELECT() > 0 ) {
        while ( sql_query.NEXT() EQ true ) {
            if ( sql_query.VALUE(0).toInt() NE p_urun_id ) {
                if ( sql_query.VALUE(1).toString() EQ m_ui->lineEdit_urun_kodu->text() ) {
                    if ( m_ui->lineEdit_urun_kodu->text().isEmpty() EQ false ) {
                        MSG_WARNING(  tr("Bu ürün kodu ile bir ürün veya hizmet var. Ürün kodunu değiştiriniz."), m_ui->lineEdit_urun_kodu );

                        return ADAK_FAIL;
                    }
                }
                else if ( sql_query.VALUE(2).toString() EQ m_ui->lineEdit_urun_kodu->text() ) {
                    if ( m_ui->lineEdit_urun_kodu->text().isEmpty() EQ false ) {
                        MSG_WARNING(  tr ( "Bu ürün kodu ile başka bir ürün veya hizmet barkod numarası kayıtlı. Ürün kodunu değiştiriniz." ), m_ui->lineEdit_urun_kodu );

                        return ADAK_FAIL;
                    }

                }
                else if ( sql_query.VALUE(1).toString() EQ m_ui->lineEdit_barkod_numarasi->text() ) {
                    if ( m_ui->lineEdit_barkod_numarasi->text().isEmpty() EQ false ) {
                        MSG_WARNING(  tr ( "Bu barkod numarası ile başka bir ürün veya hizmet kodu kayıtlı. Barkod numarasını değiştiriniz." ), m_ui->lineEdit_barkod_numarasi );

                        return ADAK_FAIL;
                    }
                }
                else {
                    if ( m_ui->lineEdit_barkod_numarasi->text().isEmpty() EQ false ) {
                        MSG_WARNING(  tr ( "Bu barkod numarası başka bir üründe veya hzimette barkod numarası olarak kayıtlı. Barkod numarasını değiştiriniz." ), m_ui->lineEdit_barkod_numarasi );

                        return ADAK_FAIL;
                    }
                }
            }
        }
    }

    int old_stok_takip_sekli = -1;
    int new_stok_takip_sekli = STK_GET_STOK_TAKIP_SEKLI_ENUM ( m_ui->comboBox_stok_takip_sekli->currentText() );

    sql_query.PREPARE_SELECT("stk_urunler","stok_takip_sekli","urun_id = :urun_id ");
    sql_query.SET_VALUE(":urun_id", p_urun_id );
    if ( sql_query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr("Stok takip şekli veritabanından alınamadı."), NULL );
        return ADAK_FAIL;
    }
    sql_query.NEXT();
    old_stok_takip_sekli = sql_query.VALUE(0).toInt();

    if ( old_stok_takip_sekli NE new_stok_takip_sekli ) {
        sql_query.PREPARE_SELECT("stk_fis_satirlari","urun_id","urun_id = :urun_id");
        sql_query.SET_VALUE(":urun_id", p_urun_id );
        if ( sql_query.SELECT() > 0 ) {
            MSG_WARNING( tr("Bu ürünü içeren stok fişi veya irsaliye var. Stok takip şekli değiştirilemez."), NULL );
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::UPDATE_RECORD
***************************************************************************************/

void STOK_URUN_KARTI_FORMU::UPDATE_RECORD ( int p_urun_id )
{

    QByteArray fotograf;
    if ( m_fotograf_path.isEmpty() EQ false ) {
        fotograf = ADAK_PIXMAP_2_BYTE_ARRAY( m_fotograf_path );
    }
    else {
        fotograf = QByteArray();
    }

    SQL_QUERY sql_query ( DB );

    if ( m_stok_takip_sekli EQ ENUM_SERI_NO_ILE_TAKIP ) {

        sql_query.PREPARE_SELECT("stk_urunler","stok_takip_sekli","urun_id = :urun_id");
        sql_query.SET_VALUE(":urun_id", p_urun_id );

        if ( sql_query.SELECT() EQ 0 ) {
            return;
        }
        sql_query.NEXT();
        int old_stok_takip_sekli = sql_query.VALUE(0).toInt();

        if ( old_stok_takip_sekli NE m_stok_takip_sekli ) {

            QString     URUN_BIRIM_ADLARI               [URUN_BIRIMLERI_SAYISI];
            double      URUN_BIRIM_KATSAYILARI          [URUN_BIRIMLERI_SAYISI];
            double      URUN_CAPRAZ_BIRIMLER            [URUN_BIRIMLERI_SAYISI];
            int         GORUNTULENECEK_URUN_BIRMLERI    [URUN_BIRIMLERI_SAYISI];

            for ( int i = 0 ; i < URUN_BIRIMLERI_SAYISI ; i++ ) {

                URUN_BIRIM_ADLARI[i]             = "";
                URUN_BIRIM_KATSAYILARI[i]        = 0.0;
                URUN_CAPRAZ_BIRIMLER[i]          = 0.0;
                GORUNTULENECEK_URUN_BIRMLERI [i] = 0;

            }

            sql_query.PREPARE_UPDATE("stk_urunler","urun_id","urun_birimleri_array          ,"
                                     "urun_birim_katsayilari_array   ,"
                                     "urun_capraz_birimler_array    ,"
                                     "gorunecek_urn_birimleri_array ","urun_id = :urun_id");
            sql_query.SET_VALUE(":urun_birimleri_array" , PACK_STRING_ARRAY(URUN_BIRIM_ADLARI , URUN_BIRIMLERI_SAYISI));
            sql_query.SET_VALUE(":urun_birim_katsayilari_array" , PACK_DOUBLE_ARRAY(URUN_BIRIM_KATSAYILARI , URUN_BIRIMLERI_SAYISI));
            sql_query.SET_VALUE(":urun_capraz_birimler_array" , PACK_DOUBLE_ARRAY(URUN_CAPRAZ_BIRIMLER , URUN_BIRIMLERI_SAYISI));
            sql_query.SET_VALUE(":gorunecek_urn_birimleri_array" , PACK_INT_ARRAY(GORUNTULENECEK_URUN_BIRMLERI , URUN_BIRIMLERI_SAYISI));
            sql_query.SET_VALUE(":urun_id"                       , p_urun_id);

            sql_query.UPDATE();

        }

    }

    sql_query.PREPARE_UPDATE ( "stk_urunler","urun_id","urun_kodu   , "
                               "urun_adi             , "
                               "barkod_numarasi      , "
                               "alis_fiyati          , "
                               "satis_fiyati         , "
                               "urun_alis_doviz_id   , "
                               "urun_satis_doviz_id  , "
                               "stok_takip_sekli     , "
                               "urun_temel_birimi    , "
                               "min_stok_seviyesi    , "
                               "kritik_stok_seviyesi ,"
                               "aciklama             , "
                               "alis_otv_orani       , "
                               "alis_kdv_orani       , "
                               "satis_otv_orani      , "
                               "satis_kdv_orani      , "
                               "muh_hesap_id         , "
                               "fotograf               ",
                               "urun_id        = :urun_id" );

    sql_query.SET_VALUE ( ":urun_kodu"            , m_ui->lineEdit_urun_kodu->text()                                );
    sql_query.SET_VALUE ( ":urun_adi"             , m_ui->lineEdit_urun_adi->text()                                 );
    sql_query.SET_VALUE ( ":barkod_numarasi"      , m_ui->lineEdit_barkod_numarasi->text()                          );
    sql_query.SET_VALUE ( ":alis_fiyati"          , m_ui->commaEdit_alis_fiyati->GET_DOUBLE()                       );
    sql_query.SET_VALUE ( ":satis_fiyati"         , m_ui->commaEdit_satis_fiyati->GET_DOUBLE()                      );
    sql_query.SET_VALUE ( ":urun_alis_doviz_id"   , DVZ_GET_DOVIZ_ID ( m_ui->comboBox_urun_alis_doviz->currentText())   );
    sql_query.SET_VALUE ( ":urun_satis_doviz_id"  , DVZ_GET_DOVIZ_ID ( m_ui->comboBox_urun_satis_doviz->currentText())  );
    sql_query.SET_VALUE ( ":stok_takip_sekli"     , m_stok_takip_sekli                                              );
    sql_query.SET_VALUE ( ":urun_temel_birimi"    , m_ui->comboBox_temel_urun_birimi->currentText()                 );
    sql_query.SET_VALUE ( ":min_stok_seviyesi"    , m_ui->commaEdit_minimum_stok_seviyesi->GET_DOUBLE()             );
    sql_query.SET_VALUE ( ":kritik_stok_seviyesi" , m_ui->commaEdit_kritik_stok_seviyesi->GET_DOUBLE()              );
    sql_query.SET_VALUE ( ":aciklama"             , m_ui->limitedTextEdit_aciklama->toPlainText()                   );
    sql_query.SET_VALUE ( ":alis_otv_orani"       , m_ui->comboBox_alis_otv_orani->currentText().toDouble()         );
    sql_query.SET_VALUE ( ":alis_kdv_orani"       , m_ui->comboBox_alis_kdv_orani->currentText().toDouble()         );
    sql_query.SET_VALUE ( ":satis_otv_orani"      , m_ui->comboBox_satis_otv_orani->currentText().toDouble()        );
    sql_query.SET_VALUE ( ":satis_kdv_orani"      , m_ui->comboBox_satis_kdv_orani->currentText().toDouble()        );
    sql_query.SET_VALUE ( ":muh_hesap_id"         , m_muh_hesap_id                                                  );
    sql_query.SET_VALUE ( ":fotograf"             , fotograf                                                        );
    sql_query.SET_VALUE ( ":urun_id"              , p_urun_id                                                       );
    sql_query.UPDATE();

    GRP_KAYDI_GRUPLARA_EKLE(E9_PROGRAMI , STOK_MODULU , p_urun_id, &m_grup_idleri);

    QString log_detaylari = "Ürün Kodu / Adı : " + m_ui->lineEdit_urun_kodu->text() + "  " +
                            m_ui->lineEdit_urun_adi->text();
    E9_LOG_KAYDI_EKLE(STOK_MODULU , LOG_STK_URUNLER , LOG_ISLEM_UPDATE , log_detaylari);

}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::CHECK_DELETE
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::CHECK_DELETE ( int p_urun_id )
{
    Q_UNUSED ( p_urun_id );

    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("stk_fis_satirlari","fis_satiri_id","urun_id = :urun_id");
    select_query.SET_VALUE(":urun_id" , p_urun_id);

    if(select_query.SELECT() NE 0 ) {
        MSG_WARNING(  tr ( "Ürün işlem görmüş,silinemez" ), NULL );
        return ADAK_FAIL;
    }

    int fis_satiri_id = FAT_GET_FATURA_FIS_SATIRI_ID( p_urun_id );

    if ( fis_satiri_id NE 0 ) {
        MSG_WARNING(  tr ( "Ürün işlem görmüş,silinemez" ), NULL );
        return ADAK_FAIL;
    }


   int msg_secim = MSG_YES_NO( tr("Seçili olan stok kayıtlarlardan silinecektir. Emin misiniz ?"), NULL );

    if ( msg_secim EQ ADAK_NO ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::DELETE_RECORD
***************************************************************************************/

void STOK_URUN_KARTI_FORMU::DELETE_RECORD ( int p_urun_id )
{
    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT("stk_urunler","urun_kodu,urun_adi","urun_id = :urun_id");
    sql_query.SET_VALUE(":urun_id" , p_urun_id);

    if(sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    QString urun_kodu = sql_query.VALUE(0).toString();
    QString urun_adi  = sql_query.VALUE(1).toString();

    sql_query.PREPARE_DELETE("stk_urunler","urun_id = :urun_id");
    sql_query.SET_VALUE(":urun_id", p_urun_id );
    sql_query.DELETE();

    GRP_KAYDIN_GRUPLARINI_SIL(E9_PROGRAMI , STOK_MODULU , p_urun_id);

    QString log_detaylari = "Ürün Kodu / Adı : " + urun_kodu + "  " + urun_adi;
    E9_LOG_KAYDI_EKLE(STOK_MODULU , LOG_STK_URUNLER , LOG_ISLEM_DELETE , log_detaylari);
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::FIND_FIRST_RECORD()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT("stk_urunler","urun_id","hizmet_mi = :hizmet_mi ");
    sql_query.SET_VALUE(":hizmet_mi",0);
    if ( sql_query.SELECT("urun_kodu ASC",0,1) EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::FIND_LAST_RECORD ()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT("stk_urunler","urun_id","hizmet_mi = :hizmet_mi ");
    sql_query.SET_VALUE(":hizmet_mi",0);
    if ( sql_query.SELECT("urun_kodu DESC",0,1) EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::FIND_NEXT_RECORD ()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT("stk_urunler","urun_id","urun_kodu > :urun_kodu AND hizmet_mi = :hizmet_mi ");
    sql_query.SET_VALUE(":urun_kodu", m_ui->lineEdit_urun_kodu->text());
    sql_query.SET_VALUE(":hizmet_mi", 0     );

    if ( sql_query.SELECT("urun_kodu ASC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::FIND_PREV_RECORD ()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT("stk_urunler","urun_id","urun_kodu < :urun_kodu AND hizmet_mi = :hizmet_mi ");
    sql_query.SET_VALUE(":urun_kodu", m_ui->lineEdit_urun_kodu->text());
    sql_query.SET_VALUE(":hizmet_mi", 0     );

    if ( sql_query.SELECT("urun_kodu DESC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::FIND_RECORD
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::FIND_RECORD()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT("stk_urunler","urun_id","urun_kodu = :urun_kodu AND hizmet_mi = :hizmet_mi ");
    sql_query.SET_VALUE(":urun_kodu", m_ui->lineEdit_urun_kodu->text());
    sql_query.SET_VALUE(":hizmet_mi", 0 );
    if ( sql_query.SELECT() EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}


/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::SELECT_RECORD
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::SELECT_RECORD()
{
    return OPEN_STOK_URUN_ARAMA ( ENUM_URUN_KARTI,-1, "",-1, this );

}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::LOCK_RECORD
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::LOCK_RECORD ( int p_urun_id )
{
    return DB->LOCK_ROW ( "urun_id","stk_urunler", QString ( "urun_id = %1").arg ( p_urun_id ) );
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::UNLOCK_RECORD
***************************************************************************************/

void STOK_URUN_KARTI_FORMU::UNLOCK_RECORD( int p_urun_id )
{
    DB->UNLOCK_ROW ( "urun_id","stk_urunler", QString ( "urun_id = %1").arg ( p_urun_id ) );
}


/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::SAVER_BUTTON_CLICKED
***************************************************************************************/

void STOK_URUN_KARTI_FORMU::SAVER_BUTTON_CLICKED ( QAbstractButton * button, int p_urun_id )
{
    if( button EQ m_ui->button_diger_urun_birimleri ) {
        if ( m_stok_takip_sekli EQ ENUM_SERI_NO_ILE_TAKIP ) {
            MSG_WARNING(  tr("Seri no ile takip edilen ürünler içın stok birimi 'Adet'dir ve yeni birim eklenemez."), NULL );
            return;
        }
        OPEN_STOK_URUN_BIRIMLERI_BATCH ( p_urun_id, this );
        URUN_BIRIMLERINI_GORUNTULE     ( p_urun_id );
    }
    else if( button EQ m_ui->pushButton_urun_hareketleri ) {
        OPEN_STOK_URUN_HAREKETLERI_ARAMA( p_urun_id, this );
    }
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::SET_WIDGET_ARRAYS
***************************************************************************************/

void STOK_URUN_KARTI_FORMU::SET_WIDGET_ARRAYS ()
{
    M_COMMAEDIT_DIGER_BIRIM_MIKTARLARI     = new QCommaEdit * [URUN_BIRIMLERI_SAYISI];

    M_COMMAEDIT_DIGER_BIRIM_MIKTARLARI[0]  = m_ui->commaEdit_miktar_1;
    M_COMMAEDIT_DIGER_BIRIM_MIKTARLARI[1]  = m_ui->commaEdit_miktar_2;
    M_COMMAEDIT_DIGER_BIRIM_MIKTARLARI[2]  = m_ui->commaEdit_miktar_3;
    M_COMMAEDIT_DIGER_BIRIM_MIKTARLARI[3]  = m_ui->commaEdit_miktar_4;

    M_LABEL_DIGER_BIRIM_ADLARI             = new QLabel * [URUN_BIRIMLERI_SAYISI];

    M_LABEL_DIGER_BIRIM_ADLARI[0]          = m_ui->label_birim_1;
    M_LABEL_DIGER_BIRIM_ADLARI[1]          = m_ui->label_birim_2;
    M_LABEL_DIGER_BIRIM_ADLARI[2]          = m_ui->label_birim_3;
    M_LABEL_DIGER_BIRIM_ADLARI[3]          = m_ui->label_birim_4;

    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
        M_COMMAEDIT_DIGER_BIRIM_MIKTARLARI[i]->hide();
        M_LABEL_DIGER_BIRIM_ADLARI[i]->hide();
    }
}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::URUN_BIRIMLERINI_GORUNTULE
***************************************************************************************/

void STOK_URUN_KARTI_FORMU::URUN_BIRIMLERINI_GORUNTULE ( int p_urun_id )
{
    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
        M_COMMAEDIT_DIGER_BIRIM_MIKTARLARI[i]->hide();
        M_LABEL_DIGER_BIRIM_ADLARI[i]->hide();
    }

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("stk_urunler","urun_birimleri_array, urun_birim_katsayilari_array, "
                             "urun_capraz_birimler_array, gorunecek_urn_birimleri_array","urun_id = :urun_id");

    sql_query.SET_VALUE ( ":urun_id", p_urun_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    sql_query.NEXT();

    QString     urun_birim_adlari_str               = sql_query.VALUE(0).toString();
    QString     urun_birim_katsayilari_str          = sql_query.VALUE(1).toString();
    QString     urun_capraz_birimler_str            = sql_query.VALUE(2).toString();
    QString     goruntulenecek_urun_birimleri_str   = sql_query.VALUE(3).toString();

    QString     URUN_BIRIM_ADLARI               [URUN_BIRIMLERI_SAYISI];
    double      URUN_BIRIM_KATSAYILARI          [URUN_BIRIMLERI_SAYISI];
    int         URUN_CAPRAZ_BIRIMLER            [URUN_BIRIMLERI_SAYISI];
    int         GORUNTULENECEK_URUN_BIRMLERI    [URUN_BIRIMLERI_SAYISI];


    UNPACK_STRING_ARRAY ( urun_birim_adlari_str,             URUN_BIRIM_ADLARI,            URUN_BIRIMLERI_SAYISI );
    UNPACK_DOUBLE_ARRAY ( urun_birim_katsayilari_str,        URUN_BIRIM_KATSAYILARI,       URUN_BIRIMLERI_SAYISI );
    UNPACK_INT_ARRAY    ( urun_capraz_birimler_str,          URUN_CAPRAZ_BIRIMLER,         URUN_BIRIMLERI_SAYISI );
    UNPACK_INT_ARRAY    ( goruntulenecek_urun_birimleri_str, GORUNTULENECEK_URUN_BIRMLERI, URUN_BIRIMLERI_SAYISI );

    for ( int i = 0; i < URUN_BIRIMLERI_SAYISI; i++ ) {
        if ( GORUNTULENECEK_URUN_BIRMLERI[i] EQ 1 ) {
            M_COMMAEDIT_DIGER_BIRIM_MIKTARLARI[i]->show();
            M_LABEL_DIGER_BIRIM_ADLARI[i]->show();
            M_LABEL_DIGER_BIRIM_ADLARI[i]->setText(URUN_BIRIM_ADLARI[i]);
            double birim_miktari = 0.0;
            double katsayi       = 1.0;
            int    capraz_birim  = URUN_CAPRAZ_BIRIMLER[i];
            katsayi     *= URUN_BIRIM_KATSAYILARI[i];
            while ( capraz_birim > 0 ) {
                katsayi     *= URUN_BIRIM_KATSAYILARI[capraz_birim -1 ];
                capraz_birim = URUN_CAPRAZ_BIRIMLER  [capraz_birim -1 ];
            }
            birim_miktari = ROUND( m_ui->commaEdit_temel_miktar->GET_DOUBLE() / katsayi );
            M_COMMAEDIT_DIGER_BIRIM_MIKTARLARI[i]->SET_DOUBLE(birim_miktari);
            M_LABEL_DIGER_BIRIM_ADLARI[i]->setText(URUN_BIRIM_ADLARI[i]);
        }
    }
}


/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::SEARCH_EDIT_CLICKED
***************************************************************************************/

void STOK_URUN_KARTI_FORMU::SEARCH_EDIT_CLICKED ( QWidget * p_widget, QLineEdit * p_lineEdit )
{
    Q_UNUSED (p_widget);
    m_muh_hesap_id = OPEN_MUH_HESAP_ARAMA( p_lineEdit->text(),this,true);

    if ( m_muh_hesap_id > 0 ) {
        p_lineEdit->setText ( MUH_GET_HESAP_KODU ( m_muh_hesap_id));
    }

}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::CHANGER_BUTTON_CLICKED()
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::CHANGER_BUTTON_CLICKED(QAbstractButton *button)
{
    Q_UNUSED(button);


    if ( button EQ m_ui->pushButton_gruplar ) {
        if ( OPEN_GRUP_SECIM( E9_PROGRAMI,STOK_MODULU , &m_grup_idleri , DB , this ) EQ true ) {
            QStringList grup_adlari;
            QStringList grup_kodlari;

            m_ui->textEdit_gruplar->setText(GRP_GRUP_ADLARINI_BUL(&m_grup_idleri , &grup_adlari, &grup_kodlari));

            return ADAK_RECORD_CHANGED;
        }
    }
    else if ( button EQ m_ui->pushButton_fotograf_sec ) {
        return FOTOGRAF_SEC ();
    }
    return ADAK_RECORD_UNCHANGED;

}

/**************************************************************************************
                   STOK_URUN_KARTI_FORMU::FILL_URUN_TEMEL_BIRIMLERI
***************************************************************************************/

void STOK_URUN_KARTI_FORMU::FILL_URUN_TEMEL_BIRIMLERI()
{
    SQL_QUERY query (DB);

    query.PREPARE_SELECT("stk_urunler","urun_temel_birimi");

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    QStringList urun_temel_birimi;

    while( query.NEXT() EQ true ) {
        if ( urun_temel_birimi.contains(query.VALUE(0).toString()) EQ false ) {
            urun_temel_birimi << query.VALUE(0).toString();
        }
    }

    m_ui->comboBox_temel_urun_birimi->clear();
    m_ui->comboBox_temel_urun_birimi->addItems(urun_temel_birimi);
    m_ui->comboBox_temel_urun_birimi->setCurrentIndex(-1);
    m_ui->label_temel_birim->clear();
}

/**************************************************************************************
                      STOK_URUN_KARTI_FORMU::FOTOGRAF_SEC
***************************************************************************************/

int STOK_URUN_KARTI_FORMU::FOTOGRAF_SEC()
{
    ADAK_ADD_PIXMAP_TO_BUTTON( m_ui->pushButton_fotograf_sec ,&m_fotograf_path, this );

    if ( m_fotograf_path.isEmpty() EQ true ) {
        return ADAK_RECORD_UNCHANGED;
    }

    return ADAK_RECORD_CHANGED;
}

/**************************************************************************************
                      STOK_URUN_KARTI_FORMU::FOTOGRAF_CLEAR
***************************************************************************************/

void STOK_URUN_KARTI_FORMU::FOTOGRAF_CLEAR()
{
     m_ui->pushButton_fotograf_sec->setIcon( QIcon( m_fotograf_path ) );
     m_ui->pushButton_fotograf_sec->setText( tr( "Fotoğraf Seç" ) );
}

