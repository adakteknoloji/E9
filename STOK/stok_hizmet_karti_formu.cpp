#include "stok_hizmet_karti_formu_class.h"
#include "ui_stok_hizmet_karti_formu.h"
#include "adak_sql.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "stok_enum.h"
#include "muh_console_utils.h"
#include "e9_enum.h"
#include "e9_log.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "fat_console_utils.h"
#include "sube_console_utils.h"
#include "muh_hesap_arama_open.h"
#include "stok_urun_arama_open.h"
#include "dvz_gui_utils.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "stok_console_utils.h"
#include "stok_urun_hareketleri_arama_open.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_STOK_HIZMET_KARTI_FORMU
***************************************************************************************/


void OPEN_STOK_HIZMET_KARTI_FORMU ( QWidget * parent )
{
    STOK_HIZMET_KARTI_FORMU * F = new STOK_HIZMET_KARTI_FORMU(parent);
    F->SHOW( FULL_SCREEN );
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::STOK_HIZMET_KARTI_FORMU
***************************************************************************************/

STOK_HIZMET_KARTI_FORMU::STOK_HIZMET_KARTI_FORMU ( QWidget * parent ) : FORM_KERNEL ( parent ), m_ui ( new Ui:: STOK_HIZMET_KARTI_FORMU )
{
    m_ui->setupUi           ( this );
    START_FORM_KERNEL       ( this, DB );
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::START_FORM_KERNEL
***************************************************************************************/

void  STOK_HIZMET_KARTI_FORMU::SETUP_FORM()
{
    SET_HELP_PAGE       ( "stok-islemleri_hizmet-karti" );
    SET_PAGE_TITLE      ( tr ("STK - HİZMET KARTI") );

    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_buttons_widget );

    SET_SETTING_NAME        ( "FATURA_HIZMET_KARTI" );
    SET_FIRST_FOCUS_WIDGET  ( m_ui->lineEdit_hizmet_kodu );

    m_temel_para_birimi_kodu = DVZ_GET_TEMEL_PARA_BIRIMI_KODU ();

    REGISTER_SAVER_BUTTON( m_ui->pushButton_hizmetin_hareketleri );

    if ( E9_KULLANICI_URUN_HAREKETLERINI_GOREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_hizmetin_hareketleri->setVisible( false );
    }

    // [ 1 ] Hassasiyet setleniyor
    int stok_birim_fiyat_hassasiyeti = STK_GET_BIRIM_HASSASIYETI();

    m_ui->commaEdit_alis_fiyati->SET_PRECISION( stok_birim_fiyat_hassasiyeti );
    m_ui->commaEdit_satis_fiyati->SET_PRECISION( stok_birim_fiyat_hassasiyeti );
    // [/1]
     

    DVZ_FILL_COMBOBOX         ( m_ui->comboBox_alis_doviz_cinsi   );
    DVZ_FILL_COMBOBOX         ( m_ui->comboBox_satis_doviz_cinsi  );
    E9_FILL_KDV_ORANLARI_COMBOBOX  ( m_ui->comboBox_alis_kdv_orani          );
    E9_FILL_KDV_ORANLARI_COMBOBOX  ( m_ui->comboBox_satis_kdv_orani          );

    m_ui->lineEdit_hizmet_kodu->setMaxLength                ( 30 );
    m_ui->lineEdit_barkod_no->setMaxLength                  ( 50 );
    m_ui->lineEdit_hizmet_ismi->setMaxLength                ( 50 );

    m_ui->limitedTextEdit->SET_LINE_LIMIT(5);
    m_ui->limitedTextEdit->SET_MAX_STRING_LENGTH(512);
    m_ui->lineEdit_hizmet_birimi->setMaxLength(20);

    m_muh_hesap_id = 0;

    m_ui->comboBox_hizmet_turu->addItems(QStringList() << QObject::tr("Alınan Hizmet") << QObject::tr("Satılan Hizmet") << QObject::tr( "Alınan / Satılan Hizmet") );


    DISABLE_CLEAR_ON_WIDGET(m_ui->searchEdit_muh_hesap_kodu);
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_muh_hesap_ismi);

    QString hizmetler_hesap_kodu, hizmetler_hesap_adi;

    m_muh_hesap_id = SUBE_GET_SUBE_ENT_HESAP_ID( SUBE_GET_SUBE_ID(), "hizmetler_ent_hesap_id", &hizmetler_hesap_kodu, &hizmetler_hesap_adi );

    m_ui->searchEdit_muh_hesap_kodu->SET_TEXT   ( hizmetler_hesap_kodu );
    m_ui->lineEdit_muh_hesap_ismi->setText      ( hizmetler_hesap_adi  );

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 0 ) {
        m_ui->searchEdit_muh_hesap_kodu->setHidden(true);
        m_ui->lineEdit_muh_hesap_ismi->setHidden(true);
        m_ui->label_muhasebe_hesabi->setHidden(true);
    }
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void  STOK_HIZMET_KARTI_FORMU::CLEAR_FORM_MEMBERS ()
{

}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::NEW_RECORD
***************************************************************************************/

void  STOK_HIZMET_KARTI_FORMU::NEW_RECORD()
{
    m_ui->comboBox_alis_doviz_cinsi->setCurrentIndex(m_ui->comboBox_alis_doviz_cinsi->findText(m_temel_para_birimi_kodu));
    m_ui->comboBox_satis_doviz_cinsi->setCurrentIndex(m_ui->comboBox_satis_doviz_cinsi->findText(m_temel_para_birimi_kodu));
}
/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::GET_RECORD
***************************************************************************************/

int STOK_HIZMET_KARTI_FORMU::GET_RECORD ( int record_id )
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("stk_urunler" , "urun_kodu,urun_adi,barkod_numarasi,alis_fiyati,satis_fiyati,"
                                "urun_alis_doviz_id,urun_satis_doviz_id,aciklama,"
                                "alis_kdv_orani,muh_hesap_id,urun_temel_birimi,hizmet_turu, satis_kdv_orani" , "urun_id = :urun_id");

    select_query.SET_VALUE(":urun_id" , record_id);

    if(select_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }
    select_query.NEXT();

    m_ui->lineEdit_hizmet_kodu->setText(select_query.VALUE(0).toString());
    m_ui->lineEdit_hizmet_ismi->setText(select_query.VALUE(1).toString());
    m_ui->lineEdit_barkod_no->setText(select_query.VALUE(2).toString());
    m_ui->commaEdit_alis_fiyati->SET_DOUBLE(select_query.VALUE(3).toDouble());
    m_ui->commaEdit_satis_fiyati->SET_DOUBLE(select_query.VALUE(4).toDouble());
    m_ui->comboBox_alis_doviz_cinsi->setCurrentIndex(m_ui->comboBox_alis_doviz_cinsi->findText(DVZ_GET_DOVIZ_KODU(select_query.VALUE(5).toInt())));
    m_ui->comboBox_satis_doviz_cinsi->setCurrentIndex(m_ui->comboBox_satis_doviz_cinsi->findText(DVZ_GET_DOVIZ_KODU(select_query.VALUE(6).toInt())));
    m_ui->limitedTextEdit->setPlainText(select_query.VALUE(7).toString());

    m_ui->comboBox_alis_kdv_orani->setCurrentIndex(m_ui->comboBox_alis_kdv_orani->findText(select_query.VALUE(8).toString()));
    m_ui->comboBox_satis_kdv_orani->setCurrentIndex(m_ui->comboBox_satis_kdv_orani->findText(select_query.VALUE(12).toString()));

    m_muh_hesap_id = select_query.VALUE(9).toInt();
    m_ui->lineEdit_hizmet_birimi->setText(select_query.VALUE(10).toString());

    int hizmet_turu = select_query.VALUE(11).toInt();

    m_ui->comboBox_hizmet_turu->setCurrentIndex(m_ui->comboBox_hizmet_turu->findText(STK_GET_HIZMET_TURU_STRING(hizmet_turu)));


    if ( m_muh_hesap_id NE 0 ) {

        QString tam_hesap_kodu,hesap_ismi;

        MUH_GET_HESAP_KODU_HESAP_ISMI(m_muh_hesap_id , tam_hesap_kodu , hesap_ismi);

        m_ui->searchEdit_muh_hesap_kodu->SET_TEXT(tam_hesap_kodu);
        m_ui->lineEdit_muh_hesap_ismi->setText(hesap_ismi);

    }

    return ADAK_OK;

}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::CHECK_VAR
***************************************************************************************/

int STOK_HIZMET_KARTI_FORMU::CHECK_VAR (QObject * object)
{
    if ( object EQ m_ui->searchEdit_muh_hesap_kodu ) {

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

        m_ui->lineEdit_muh_hesap_ismi->setText (MUHASEBE_HESABI->hesap_ismi);
        m_muh_hesap_id = MUHASEBE_HESABI->hesap_id;
    }
    else if ( object EQ m_ui->comboBox_hizmet_turu ) {

        m_ui->label_satis->setVisible               ( true );
        m_ui->commaEdit_satis_fiyati->setVisible    ( true );
        m_ui->comboBox_satis_kdv_orani->setVisible  ( true );
        m_ui->label_satis_kdv->setVisible           ( true );
        m_ui->comboBox_satis_doviz_cinsi->setVisible( true );
        m_ui->label_satis_yuzde->setVisible         ( true );

        m_ui->label_alis_fiyati->setVisible         ( true );
        m_ui->commaEdit_alis_fiyati->setVisible     ( true );
        m_ui->comboBox_alis_kdv_orani->setVisible   ( true );
        m_ui->label_alis_kdv_orani->setVisible      ( true );
        m_ui->comboBox_alis_doviz_cinsi->setVisible ( true );
        m_ui->label_alis_yuzde->setVisible          ( true );

        if ( STK_GET_HIZMET_TURU_ENUM ( m_ui->comboBox_hizmet_turu->currentText() ) EQ ENUM_ALINAN_HIZMET ) {
            m_ui->label_satis->setVisible( false );
            m_ui->commaEdit_satis_fiyati->setVisible( false );
            m_ui->comboBox_satis_kdv_orani->setVisible( false );
            m_ui->label_satis_kdv->setVisible( false );
            m_ui->comboBox_satis_doviz_cinsi->setVisible( false );
            m_ui->label_satis_yuzde->setVisible( false );
        }
        else if ( STK_GET_HIZMET_TURU_ENUM ( m_ui->comboBox_hizmet_turu->currentText() ) EQ ENUM_SATILAN_HIZMET ) {
            m_ui->label_alis_fiyati->setVisible( false );
            m_ui->commaEdit_alis_fiyati->setVisible( false );
            m_ui->comboBox_alis_kdv_orani->setVisible( false );
            m_ui->label_alis_kdv_orani->setVisible( false );
            m_ui->comboBox_alis_doviz_cinsi->setVisible( false );
            m_ui->label_alis_yuzde->setVisible( false );
        }

    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::CHECK_EMPTY
***************************************************************************************/

int STOK_HIZMET_KARTI_FORMU::CHECK_EMPTY ()
{
    if ( m_ui->lineEdit_hizmet_kodu->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Hizmet kodunu girmelisiniz!.."), m_ui->lineEdit_hizmet_kodu );

        return ADAK_FAIL;
    }
    if ( m_ui->lineEdit_hizmet_ismi->text().isEmpty() EQ true ) {
        MSG_WARNING( tr("Hizmet İsmini boş bırakamazsınız!.."), m_ui->lineEdit_hizmet_ismi );

        return ADAK_FAIL;
    }

    if ( m_muh_hesap_id < 1 ) {
        MSG_WARNING(  tr ( "Hizmetin entegre olacağı muhasebe hesabını boş bırakamazsınız." ),m_ui->searchEdit_muh_hesap_kodu );

        return ADAK_FAIL;
    }

    if ( m_ui->comboBox_hizmet_turu->currentIndex() EQ -1 ) {
        MSG_WARNING(QObject::tr("Hizmet türünü boş bırakamazsınız."),m_ui->comboBox_hizmet_turu);

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::CHECK_ADD
***************************************************************************************/

int STOK_HIZMET_KARTI_FORMU::CHECK_ADD ()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT    ( "stk_urunler ","urun_id, urun_kodu, barkod_numarasi",
                              "urun_kodu               = :urun_kodu_1       "
                              "OR    urun_kodu         = :urun_kodu_2       "
                              "OR    barkod_numarasi   = :barkod_numarasi_1 "
                              "OR    barkod_numarasi   = :barkod_numarasi_2 ");

    query.SET_VALUE ( ":urun_kodu_1",       m_ui->lineEdit_hizmet_kodu->text()       );
    query.SET_VALUE ( ":urun_kodu_2",       m_ui->lineEdit_barkod_no->text() );
    query.SET_VALUE ( ":barkod_numarasi_1", m_ui->lineEdit_hizmet_kodu->text()       );
    query.SET_VALUE ( ":barkod_numarasi_2", m_ui->lineEdit_barkod_no->text() );

    if ( query.SELECT() > 0 ) {
        while ( query.NEXT() EQ true ) {
            if ( query.VALUE(1).toString() EQ m_ui->lineEdit_hizmet_kodu->text() ) {
                if ( m_ui->lineEdit_hizmet_kodu->text().isEmpty() EQ false ) {
                    MSG_WARNING(  tr("Bu hizmet kodu ile bir hizmet veya ürün var. Hizmet kodunu değiştiriniz."), m_ui->lineEdit_hizmet_kodu );

                    return ADAK_FAIL;
                }

            }
            else if ( query.VALUE(2).toString() EQ m_ui->lineEdit_hizmet_kodu->text() ) {
                if ( m_ui->lineEdit_hizmet_kodu->text().isEmpty() EQ false ) {
                    MSG_WARNING(  tr ( "Bu hizmet kodu ile başka bir ürün veya hizmet barkod numarası kayıtlı. Hizmet kodunu değiştiriniz." ), m_ui->lineEdit_hizmet_kodu );

                    return ADAK_FAIL;
                }
            }
            else if ( query.VALUE(1).toString() EQ m_ui->lineEdit_barkod_no->text() ) {
                if ( m_ui->lineEdit_barkod_no->text().isEmpty() EQ false ) {
                   MSG_WARNING(  tr ( "Bu barkod numarası ile başka bir ürün veya hizmet kodu kayıtlı. Barkod numarasını değiştiriniz." ), m_ui->lineEdit_barkod_no );

                   return ADAK_FAIL;
                }
            }
            else {
                if ( m_ui->lineEdit_barkod_no->text().isEmpty() EQ false ) {
                    MSG_WARNING(  tr ( "Bu barkod numarası başka bir üründe vyea hizmette barkod numarası olarak kayıtlı. Barkod numarasını değiştiriniz." ), m_ui->lineEdit_barkod_no );

                    return ADAK_FAIL;
                }
            }
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::ADD_RECORD
***************************************************************************************/

int STOK_HIZMET_KARTI_FORMU::ADD_RECORD ()
{
    SQL_QUERY insert_query(DB);

    insert_query.PREPARE_INSERT("stk_urunler" , "urun_id" , "urun_kodu,urun_adi,barkod_numarasi,alis_fiyati,satis_fiyati,"
                                "urun_alis_doviz_id,urun_satis_doviz_id,aciklama,"
                                "alis_kdv_orani,hizmet_mi,muh_hesap_id,urun_temel_birimi,hizmet_turu, satis_kdv_orani ");

    insert_query.SET_VALUE(":urun_kodu"           , m_ui->lineEdit_hizmet_kodu->text());
    insert_query.SET_VALUE(":urun_adi"            , m_ui->lineEdit_hizmet_ismi->text());
    insert_query.SET_VALUE(":barkod_numarasi"     , m_ui->lineEdit_barkod_no->text());
    insert_query.SET_VALUE(":alis_fiyati"         , m_ui->commaEdit_alis_fiyati->GET_DOUBLE());
    insert_query.SET_VALUE(":satis_fiyati"        , m_ui->commaEdit_satis_fiyati->GET_DOUBLE());
    insert_query.SET_VALUE(":urun_alis_doviz_id"  , DVZ_GET_DOVIZ_ID(m_ui->comboBox_alis_doviz_cinsi->currentText()));
    insert_query.SET_VALUE(":urun_satis_doviz_id" , DVZ_GET_DOVIZ_ID(m_ui->comboBox_satis_doviz_cinsi->currentText()));
    insert_query.SET_VALUE(":aciklama"            , m_ui->limitedTextEdit->toPlainText());
    insert_query.SET_VALUE(":alis_kdv_orani"      , QVariant(m_ui->comboBox_alis_kdv_orani->currentText()).toDouble());
    insert_query.SET_VALUE(":satis_kdv_orani"     , QVariant(m_ui->comboBox_satis_kdv_orani->currentText()).toDouble());
    insert_query.SET_VALUE(":hizmet_mi"           , 1);
    insert_query.SET_VALUE(":muh_hesap_id"        , m_muh_hesap_id);
    insert_query.SET_VALUE(":urun_temel_birimi"   , m_ui->lineEdit_hizmet_birimi->text());
    insert_query.SET_VALUE(":hizmet_turu"         , STK_GET_HIZMET_TURU_ENUM(m_ui->comboBox_hizmet_turu->currentText()));

    int urun_id = insert_query.INSERT();

    QString log_detaylari = "Hizmet Kodu / Adı : " + m_ui->lineEdit_hizmet_kodu->text() + "  " + m_ui->lineEdit_hizmet_ismi->text();

    E9_LOG_KAYDI_EKLE ( FATURA_MODULU , LOG_FAT_HIZMETLER , LOG_ISLEM_ADD , log_detaylari);

    return urun_id;
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::CHECK_UPDATE
***************************************************************************************/

int STOK_HIZMET_KARTI_FORMU::CHECK_UPDATE ( int record_id )
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "stk_urunler ","urun_id, urun_kodu, barkod_numarasi",
                           "urun_kodu         = :urun_kodu_1       "
                           "OR    urun_kodu         = :urun_kodu_2       "
                           "OR    barkod_numarasi   = :barkod_numarasi_1 "
                           "OR    barkod_numarasi   = :barkod_numarasi_2 ");

    query.SET_VALUE ( ":urun_kodu_1",       m_ui->lineEdit_hizmet_kodu->text()       );
    query.SET_VALUE ( ":urun_kodu_2",       m_ui->lineEdit_barkod_no->text() );
    query.SET_VALUE ( ":barkod_numarasi_1", m_ui->lineEdit_hizmet_kodu->text()       );
    query.SET_VALUE ( ":barkod_numarasi_2", m_ui->lineEdit_barkod_no->text() );

    if ( query.SELECT() > 0 ) {
        while ( query.NEXT() EQ true ) {
            if ( query.VALUE(0).toInt() NE record_id ) {
                QString a = query.VALUE(1).toString();
                if ( a  EQ m_ui->lineEdit_hizmet_kodu->text() ) {
                    if ( m_ui->lineEdit_hizmet_kodu->text().isEmpty() EQ false ) {
                        MSG_WARNING(  tr("Bu hizmet kodu ile bir ürün veya hizmet var. Hizmet kodunu değiştiriniz."), m_ui->lineEdit_hizmet_kodu );

                        return ADAK_FAIL;
                    }
                }
                else if ( query.VALUE(2).toString() EQ m_ui->lineEdit_hizmet_kodu->text() ) {
                    if ( m_ui->lineEdit_hizmet_kodu->text().isEmpty() EQ false ) {
                        MSG_WARNING(  tr ( "Bu hizmet kodu ile başka bir ürün veya hizmet barkod numarası kayıtlı. Hizmet kodunu değiştiriniz." ), m_ui->lineEdit_hizmet_kodu );

                        return ADAK_FAIL;
                    }

                }
                else if ( query.VALUE(1).toString() EQ m_ui->lineEdit_barkod_no->text() ) {
                    if ( m_ui->lineEdit_barkod_no->text().isEmpty() EQ false ) {
                        MSG_WARNING(  tr ( "Bu barkod numarası ile başka bir hizmet veya ürün kodu kayıtlı. Barkod numarasını değiştiriniz." ), m_ui->lineEdit_barkod_no );

                        return ADAK_FAIL;
                    }
                }
                else {
                    if ( m_ui->lineEdit_barkod_no->text().isEmpty() EQ false ) {
                        MSG_WARNING(  tr ( "Bu barkod numarası başka bir hizmette veya üründe barkod numarası olarak kayıtlı. Barkod numarasını değiştiriniz." ), m_ui->lineEdit_barkod_no );

                        return ADAK_FAIL;
                    }
                }
            }
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::UPDATE_RECORD
***************************************************************************************/

void STOK_HIZMET_KARTI_FORMU::UPDATE_RECORD (int record_id)
{
    SQL_QUERY update_query(DB);

    update_query.PREPARE_UPDATE("stk_urunler","urun_id", "urun_kodu,urun_adi,"
                                "barkod_numarasi ,satis_fiyati ,"
                                "alis_fiyati ,urun_alis_doviz_id,"
                                "urun_satis_doviz_id ,aciklama ,"
                                "alis_kdv_orani ,"
                                "muh_hesap_id ,urun_temel_birimi ,"
                                "hizmet_turu, satis_kdv_orani ",
                                "urun_id = :urun_id");


    update_query.SET_VALUE(":urun_kodu"            , m_ui->lineEdit_hizmet_kodu->text());
    update_query.SET_VALUE(":urun_adi"             , m_ui->lineEdit_hizmet_ismi->text());
    update_query.SET_VALUE(":barkod_numarasi"      , m_ui->lineEdit_barkod_no->text());
    update_query.SET_VALUE(":satis_fiyati"         , m_ui->commaEdit_satis_fiyati->GET_DOUBLE());
    update_query.SET_VALUE(":alis_fiyati"          , m_ui->commaEdit_alis_fiyati->GET_DOUBLE());
    update_query.SET_VALUE(":urun_alis_doviz_id"   , DVZ_GET_DOVIZ_ID(m_ui->comboBox_alis_doviz_cinsi->currentText()));
    update_query.SET_VALUE(":urun_satis_doviz_id"  , DVZ_GET_DOVIZ_ID(m_ui->comboBox_satis_doviz_cinsi->currentText()));
    update_query.SET_VALUE(":aciklama"             , m_ui->limitedTextEdit->toPlainText());
    update_query.SET_VALUE(":alis_kdv_orani"       , QVariant(m_ui->comboBox_alis_kdv_orani->currentText()).toDouble());
    update_query.SET_VALUE(":satis_kdv_orani"      , QVariant(m_ui->comboBox_satis_kdv_orani->currentText()).toDouble());
    update_query.SET_VALUE(":muh_hesap_id"         , m_muh_hesap_id);
    update_query.SET_VALUE(":urun_temel_birimi"    , m_ui->lineEdit_hizmet_birimi->text());
    update_query.SET_VALUE(":hizmet_turu"          , STK_GET_HIZMET_TURU_ENUM(m_ui->comboBox_hizmet_turu->currentText()));
    update_query.SET_VALUE(":urun_id"              , record_id);

    update_query.UPDATE();

    QString log_detaylari = "Hizmet Kodu / Adı : " + m_ui->lineEdit_hizmet_kodu->text() + "  " + m_ui->lineEdit_hizmet_ismi->text();

    E9_LOG_KAYDI_EKLE ( FATURA_MODULU , LOG_FAT_HIZMETLER , LOG_ISLEM_UPDATE , log_detaylari);
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::CHECK_DELETE
***************************************************************************************/

int STOK_HIZMET_KARTI_FORMU::CHECK_DELETE ( int record_id )
{
    //Hizmet faturada kulllanilmis,islem gorduyse degistirilemez.

    int fis_satiri_id = FAT_GET_FATURA_FIS_SATIRI_ID( record_id );

    if( fis_satiri_id NE 0 ) {
        MSG_WARNING( tr("Hizmet fatura satırlarında kullanılmış,silemezsiniz!.."), NULL );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::DELETE_RECORD
***************************************************************************************/

void STOK_HIZMET_KARTI_FORMU::DELETE_RECORD ( int record_id )
{
    SQL_QUERY delete_query(DB);
    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT("stk_urunler","urun_kodu,urun_adi" ,"urun_id = :urun_id");
    sql_query.SET_VALUE(":urun_id" , record_id);

    if(sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    QString hizmet_kodu = sql_query.VALUE(0).toString();
    QString hizmet_ismi = sql_query.VALUE(1).toString();

    delete_query.PREPARE_DELETE("stk_urunler", "urun_id = :urun_id");
    delete_query.SET_VALUE(":urun_id" , record_id);
    delete_query.DELETE();

    QString log_detaylari = "Hizmet Kodu / Adı : " + hizmet_kodu + "  " + hizmet_ismi;

    E9_LOG_KAYDI_EKLE ( FATURA_MODULU , LOG_FAT_HIZMETLER , LOG_ISLEM_DELETE , log_detaylari);
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int STOK_HIZMET_KARTI_FORMU::FIND_FIRST_RECORD()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("stk_urunler","urun_id" ,"hizmet_mi = :hizmet_mi ");
    select_query.SET_VALUE(":hizmet_mi" , 1);

    if (select_query.SELECT("urun_kodu ASC" , 0,1) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int STOK_HIZMET_KARTI_FORMU::FIND_LAST_RECORD ()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("stk_urunler","urun_id" ,"hizmet_mi = :hizmet_mi ");
    select_query.SET_VALUE(":hizmet_mi" , 1);

    if (select_query.SELECT("urun_kodu DESC" , 0,1) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int STOK_HIZMET_KARTI_FORMU::FIND_NEXT_RECORD ()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("stk_urunler" , "urun_id" , "hizmet_mi =:hizmet_mi AND "
                                "urun_kodu > :urun_kodu");

    select_query.SET_VALUE(":hizmet_mi"  , 1);
    select_query.SET_VALUE(":urun_kodu"  , m_ui->lineEdit_hizmet_kodu->text());

    if (select_query.SELECT("urun_kodu ASC" , 0 ,1) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int STOK_HIZMET_KARTI_FORMU::FIND_PREV_RECORD ()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("stk_urunler" , "urun_id" , "hizmet_mi =:hizmet_mi AND "
                                "urun_kodu < :urun_kodu");
    select_query.SET_VALUE(":hizmet_mi"  , 1);
    select_query.SET_VALUE(":urun_kodu"  , m_ui->lineEdit_hizmet_kodu->text());
    if(select_query.SELECT() EQ 0 ) {
        return 0;
    }
    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::SELECT_RECORD
***************************************************************************************/

int STOK_HIZMET_KARTI_FORMU::SELECT_RECORD()
{
    return OPEN_STOK_URUN_ARAMA ( ENUM_HIZMET_KARTI, -1, "", -1,this );
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::LOCK_RECORD
***************************************************************************************/

int STOK_HIZMET_KARTI_FORMU::LOCK_RECORD(int record_id)
{
    return DB->LOCK_ROW ( "urun_id","stk_urunler", QString ( "urun_id = %1").arg ( record_id ) );
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::UNLOCK_RECORD
***************************************************************************************/

void STOK_HIZMET_KARTI_FORMU::UNLOCK_RECORD(int record_id)
{
    DB->UNLOCK_ROW ( "urun_id","stk_urunler", QString ( "urun_id = %1").arg ( record_id ) );
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::FIND_RECORD
***************************************************************************************/

int STOK_HIZMET_KARTI_FORMU::FIND_RECORD()
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("stk_urunler","urun_id" ,"hizmet_mi = :hizmet_mi AND urun_kodu = :urun_kodu");
    select_query.SET_VALUE(":hizmet_mi" , 1);
    select_query.SET_VALUE(":urun_kodu" , m_ui->lineEdit_hizmet_kodu->text());

    if(select_query.SELECT() EQ 0 ) {
        MSG_WARNING( tr(" Aradığınız hizmet kodu bulunamadı!.."), m_ui->lineEdit_hizmet_kodu );

        return 0;
    }
    select_query.NEXT();
    return  select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::SEARCH_EDIT_CLICKED
***************************************************************************************/

void STOK_HIZMET_KARTI_FORMU::SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit)
{
    Q_UNUSED(widget);

    int muh_hesap_id = OPEN_MUH_HESAP_ARAMA( line_edit->text(), this,true );

    if ( muh_hesap_id > 0 ) {
        m_muh_hesap_id = muh_hesap_id;
        line_edit->setText(MUH_GET_HESAP_KODU(muh_hesap_id));
    }
}

/**************************************************************************************
                   STOK_HIZMET_KARTI_FORMU::SAVER_BUTTON_CLICKED
***************************************************************************************/

void STOK_HIZMET_KARTI_FORMU::SAVER_BUTTON_CLICKED(QAbstractButton *button, int record_id)
{
    if( button EQ m_ui->pushButton_hizmetin_hareketleri ) {
        OPEN_STOK_URUN_HAREKETLERI_ARAMA( record_id, this );
    }
}
