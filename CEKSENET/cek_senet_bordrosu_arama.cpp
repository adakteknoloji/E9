#include <QMessageBox>
#include "cek_senet_bordrosu_arama_class.h"
#include "ui_cek_senet_bordrosu_arama.h"
#include "adak_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_enum.h"
#include "adak_std_utils.h"
#include "cek_enum.h"
#include "cek_gui_utils.h"
#include "cek_console_utils.h"
#include "cek_senet_bordrosu_fisi_open.h"


#define     BORDRO_ID_COLUMN       0
#define     BORDRO_TARIHI_COLUMN   1
#define     BORDRO_NO_COLUMN       2
#define     ACIKLAMA_COLUMN        3
#define     TOPLAM_TUTAR_COLUMN    4

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_CEK_SENET_BORDROSU_ARAMA
***************************************************************************************/

int OPEN_CEK_SENET_BORDROSU_ARAMA ( int fis_kaynagi,int bordro_turu,int bordro_islem_turu ,
                                    int cek_bordrosu_mu , CEK_SENET_TURU p_cek_senet_turu,  QWidget * parent)
{
    int secilen_id = -1;
    CEK_SENET_BORDROSU_ARAMA * F = new CEK_SENET_BORDROSU_ARAMA ( fis_kaynagi,bordro_turu,
                                                                  bordro_islem_turu,
                                                                  cek_bordrosu_mu, p_cek_senet_turu, parent);
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_ARAMA::CEK_SENET_BORDROSU_ARAMA
***************************************************************************************/

CEK_SENET_BORDROSU_ARAMA::CEK_SENET_BORDROSU_ARAMA ( int fis_kaynagi,int bordro_turu,int bordro_islem_turu,
                                                     int cek_bordrosu_mu, CEK_SENET_TURU p_cek_senet_turu,
                                                     QWidget *parent) : ARAMA_KERNEL (parent), m_ui(new Ui::CEK_SENET_BORDROSU_ARAMA)
{
    m_modul_id              =   fis_kaynagi;
    m_bordro_turu           =   bordro_turu;
    m_bordro_islem_turu     =   bordro_islem_turu;
    m_cek_bordrosu_mu       =   cek_bordrosu_mu;
    m_cek_senet_turu        =   p_cek_senet_turu;

    m_ui->setupUi       (this ) ;
    START_ARAMA_KERNEL  (this, DB );
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_ARAMA::SETUP_FORM
***************************************************************************************/

void CEK_SENET_BORDROSU_ARAMA::SETUP_FORM()
{

    SET_HELP_PAGE( "cek-senet-islemleri");

    if ( m_cek_senet_turu EQ ENUM_CEK_SENET_ENT_FISI ) {
        SET_PAGE_TITLE ( tr ( "ÇEK-SNT - ÇEK / SENET ENTEGRASYON FİŞLERİ" ) );
        SET_HELP_PAGE( "cek-senet-islemleri_cek-senet-entegrasyon-fisleri" );

        SET_FIRST_FOCUS_WIDGET( m_ui->comboBox_ent_fis_turu );
        m_ui->islem_turu_frame->setVisible( false );

        m_ui->checkBox_ent_fis_turu->setChecked( true );
    }
    else {
        SET_PAGE_TITLE ( tr ( "ÇEK-SNT - ÇEK-SENET BORDRO ARAMA" ) );
        SET_FIRST_FOCUS_WIDGET( m_ui->comboBox_bordro_islem_turu );
        m_ui->checkBox_ent_fis_turu->setVisible( false );
        m_ui->frame_ent_fis_turu->setVisible(  false );
    }

    m_ui->adakDate_first_date->MALI_YIL_KISITI_YOK( true );
    m_ui->adakDate_second_date->MALI_YIL_KISITI_YOK( true );

    m_ui->adakDate_first_date->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_second_date->SET_DATE( MALI_ARRAY_LAST_DATE() );

    REGISTER_TABLE_WIDGET         ( m_ui->tableWidget          );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    SET_SORTING    ( false );

    SET_HEADERS(QStringList() << tr ( "Bordro Id")<<tr ( "Bordro Tarihi" )<<tr ( "Bordro No" )<<tr ( "Açıklama" )
                              << tr ( "Toplam Tutar" ) );

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );

    CEK_FILL_ENT_FIS_TURU_COMBO_BOX( m_ui->comboBox_ent_fis_turu );

    m_ui->date_frame->setEnabled        ( false );

    SET_FIND_BUTTON_NAME ( tr ( "Bordroları Göster" ) );

    m_ui->tableWidget->hideColumn ( BORDRO_ID_COLUMN );

    m_ui->tableWidget->setColumnWidth ( BORDRO_TARIHI_COLUMN, 240 );
    m_ui->tableWidget->setColumnWidth ( BORDRO_NO_COLUMN,     70  );
    m_ui->tableWidget->setColumnWidth ( ACIKLAMA_COLUMN,      300 );
    m_ui->tableWidget->setColumnWidth ( TOPLAM_TUTAR_COLUMN,  130 );


    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_bordro_islem_turu );

    CEK_FILL_COMBO_BOX_BORDRO_ISLEM_TURU(m_bordro_turu,m_ui->comboBox_bordro_islem_turu);

    m_ui->comboBox_bordro_islem_turu->insertItem(0,QObject::tr("Tüm Bordrolar"));

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_bordro_islem_turu, CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING(m_bordro_islem_turu) );

    SET_SETTING_NAME ( "CEK_SENET_BORDRO_ARAMA" );
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_ARAMA::CHECK_VAR
***************************************************************************************/

int CEK_SENET_BORDROSU_ARAMA::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->date_check_box ) {

        if ( m_ui->date_check_box->isChecked() EQ true ) {
            m_ui->date_frame->setEnabled(true);
            SET_FOCUS( m_ui->adakDate_first_date );
        }
        else {
            m_ui->date_frame->setEnabled ( false );
            m_ui->adakDate_first_date->SET_DATE(MALI_YIL_FIRST_DATE() );
        }
    }
    else if ( object EQ m_ui->comboBox_bordro_islem_turu ) {
        m_bordro_islem_turu = CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_ENUM( m_ui->comboBox_bordro_islem_turu->currentText() );
    }
    else if ( object EQ m_ui->comboBox_ent_fis_turu ) {

        if ( m_ui->comboBox_ent_fis_turu->currentText() EQ QObject::tr ( "Cari Ent. Fişi-Satıcıya Verildi( Firma Çekleri )"  )  ) {

            m_modul_id              =   CARI_MODULU;
            m_bordro_turu           =   ENUM_FIRMA_CEK_BORDROSU;
            m_bordro_islem_turu     =   ENUM_CEK_SATICIYA_VERILDI;
            m_cek_bordrosu_mu       =   1;
        }
        else if ( m_ui->comboBox_ent_fis_turu->currentText() EQ QObject::tr ( "Cari Ent. Fişi-Satıcıya Verildi( Firma Senetleri )"  ) ) {
            m_modul_id              =   CARI_MODULU;
            m_bordro_turu           =   ENUM_FIRMA_SENET_BORDROSU;
            m_bordro_islem_turu     =   ENUM_CEK_SATICIYA_VERILDI;
            m_cek_bordrosu_mu       =   0;
        }
        else if ( m_ui->comboBox_ent_fis_turu->currentText() EQ QObject::tr ( "Cari Ent. Fişi-Satıcıya Verildi( Müşteri Çekleri )"  ) ) {
            m_modul_id              =   CARI_MODULU;
            m_bordro_turu           =   ENUM_MUSTERI_CEK_BORDROSU;
            m_bordro_islem_turu     =   ENUM_CEK_SATICIYA_VERILDI;
            m_cek_bordrosu_mu       =   1;
        }
        else if ( m_ui->comboBox_ent_fis_turu->currentText() EQ QObject::tr ( "Cari Ent. Fişi-Satıcıya Verildi( Müşteri Senetleri )"  ) ) {
            m_modul_id              =   CARI_MODULU;
            m_bordro_turu           =   ENUM_MUSTERI_SENET_BORDROSU;
            m_bordro_islem_turu     =   ENUM_CEK_SATICIYA_VERILDI;
            m_cek_bordrosu_mu       =   0;
        }
        else if ( m_ui->comboBox_ent_fis_turu->currentText() EQ QObject::tr ( "Cari Ent. Fişi-Müşteriden Çek Alındı"  ) ) {
            m_modul_id              =   CARI_MODULU;
            m_bordro_turu           =   ENUM_MUSTERI_CEK_BORDROSU;
            m_bordro_islem_turu     =   ENUM_CEK_MUSTERIDEN_ALINDI;
            m_cek_bordrosu_mu       =   1;
        }
        else if ( m_ui->comboBox_ent_fis_turu->currentText() EQ QObject::tr ( "Cari Ent. Fişi-Müşteriden Senet Alındı"  ) ) {
            m_modul_id              =   CARI_MODULU;
            m_bordro_turu           =   ENUM_MUSTERI_SENET_BORDROSU;
            m_bordro_islem_turu     =   ENUM_CEK_MUSTERIDEN_ALINDI;
            m_cek_bordrosu_mu       =   0;
        }
        else if ( m_ui->comboBox_ent_fis_turu->currentText() EQ QObject::tr ( "Cari Ent. Tüm Fişler"  ) ) {
            m_modul_id              =   CARI_MODULU;
            m_bordro_turu           =   -1;
            m_bordro_islem_turu     =   ENUM_TUM_BORDROLAR;
            m_cek_bordrosu_mu       =   -1;
        }

    }
    return ADAK_OK;
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_ARAMA::CHECK_RUN
***************************************************************************************/

int CEK_SENET_BORDROSU_ARAMA::CHECK_RUN()
{
    return ADAK_OK;
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_ARAMA::SEARCH
***************************************************************************************/

void CEK_SENET_BORDROSU_ARAMA::SEARCH()
{
    SQL_QUERY f_query ( DB );

    f_query.PREPARE_SELECT ( "cek_bordrolar",
                             "bordro_id,bordro_no,bordro_tarihi,aciklama,toplam_tutar",
                             "modul_id =:modul_id" ) ;

    f_query.SET_VALUE(":modul_id", m_modul_id);

    if ( m_cek_bordrosu_mu NE -1 ) {
        f_query.AND_EKLE( "cek_bordrosu_mu = :cek_bordrosu_mu" );
        f_query.SET_VALUE(":cek_bordrosu_mu",m_cek_bordrosu_mu);
    }

    QString baslangic_tarihi = m_ui->adakDate_first_date->DATE();
    QString bitis_tarihi     = m_ui->adakDate_second_date->DATE();

    if ( m_ui->date_check_box->isChecked() EQ true  ) {
        f_query.AND_EKLE ("bordro_tarihi BETWEEN :baslangic_tarihi AND :bitis_tarihi");
        f_query.SET_VALUE(":baslangic_tarihi",baslangic_tarihi );
        f_query.SET_VALUE(":bitis_tarihi"    ,bitis_tarihi );
    }

    if ( m_bordro_islem_turu  NE ENUM_TUM_BORDROLAR ) {
        f_query.AND_EKLE ("bordro_islem_turu=:bordro_islem_turu");
        f_query.SET_VALUE(":bordro_islem_turu" , m_bordro_islem_turu );
    }

    if ( f_query.SELECT( "bordro_tarihi, bordro_no ASC " ) EQ 0 ) {
        return;
    }

    int current_row;

    while ( f_query.NEXT() EQ true ) {

        current_row = ADD_NEW_ROW();

        m_ui->tableWidget->item ( current_row, TOPLAM_TUTAR_COLUMN )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );

        m_ui->tableWidget->item ( current_row, BORDRO_ID_COLUMN     )->setText ( f_query.VALUE(0).toString() );
        m_ui->tableWidget->item ( current_row, BORDRO_NO_COLUMN     )->setText ( f_query.VALUE(1).toString() );
        m_ui->tableWidget->item ( current_row, BORDRO_TARIHI_COLUMN )->setText ( f_query.VALUE(2).toDate().toString("dd MMMM yyyy dddd"));
        m_ui->tableWidget->item ( current_row, ACIKLAMA_COLUMN      )->setText ( f_query.VALUE(3).toString() );
        m_ui->tableWidget->item ( current_row, TOPLAM_TUTAR_COLUMN  )->setText ( VIRGUL_EKLE(f_query.VALUE(4).toString()));

    }
}

/**************************************************************************************
                   CEK_SENET_BORDROSU_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int CEK_SENET_BORDROSU_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    *m_secilen_id = m_ui->tableWidget->item ( selected_row_number,0)->text().toInt();
    if ( m_cek_senet_turu EQ ENUM_CEK_SENET_FISI ) {
        return ADAK_EXIT;
    }

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "cek_bordrolar", "bordro_turu", "bordro_id = :bordro_id" );
    query.SET_VALUE     ( ":bordro_id", *m_secilen_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        m_bordro_turu = query.VALUE( 0 ).toInt();
    }

    OPEN_CEK_SENET_BORDROSU_FISI ( *m_secilen_id, m_modul_id , m_bordro_islem_turu , nativeParentWidget() , m_bordro_turu );

    return ADAK_OK;
}
