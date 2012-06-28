#include <adak_utils.h>
#include <adak_sql.h>
#include "cari_oto_ekstre_arama_open.h"
#include "cari_oto_ekstre_arama_class.h"
#include "ui_cari_oto_ekstre_arama.h"
#include "cari_fis_utils.h"
#include "fat_fatura_fisi_open.h"
#include "fat_fis_utils.h"
#include "cek_fis_utils.h"
#include "bnk_fis_utils.h"
#include "cari_hareket_fisi_open.h"
#include "bnk_hareket_fisi_open.h"
#include "muh_muhasebe_fisi_open.h"
#include "cek_senet_bordrosu_fisi_open.h"
#include "cari_console_utils.h"
#include "muh_console_utils.h"
#include "bnk_console_utils.h"
#include "cek_console_utils.h"
#include "fat_console_utils.h"
#include "cari_enum.h"
#include "cari_tahsilat_fisi_open.h"
#include "cari_odeme_fisi_open.h"
#include "cari_virman_formu_open.h"
#include "smm_makbuz_formu_open.h"
#include "smm_makbuz_utils.h"
#include "bnk_defter_fis_utils.h"
#include "banka_enum.h"
#include "bnk_virman_formu_open.h"
#include "bnk_makro_fisi_open.h"
#include "prs_con_utils.h"
#include "prs_bordro_inceleme_tree_secimi_open.h"
#include "irs_console_utils.h"
#include "stok_fisi_open.h"
#include "stok_fis_utils.h"
#include "stok_console_utils.h"
#include "fat_enum.h"
#include "e9_console_utils.h"

extern ADAK_SQL * DB;

#define BASE_FIS_ID_COLUMN          0
#define MODUL_ID_COLUMN             1
#define FIS_TURU_COLUMN             2
#define IADE_FATURASI_MI_COLUMN     3
#define BORDRO_ISLEM_TURU_COLUMN    4
#define SMM_MAKBUZ_TURU_COLUMN      5
#define FIS_ID_COLUMN               6
#define FIS_TARIHI_COLUMN           7
#define FIS_NO_COLUMN               8
#define ACIKLAMA_COLUMN             9
#define TOPLAM_BORC_COLUMN          10
#define TOPLAM_ALACAK_COLUMN        11


/**************************************************************************************
                   OPEN_CARI_OTO_EKSTRE_ARAMA
***************************************************************************************/

void OPEN_CARI_OTO_EKSTRE_ARAMA ( int p_cari_hesap_id, QWidget * parent )
{
    CARI_OTO_EKSTRE_ARAMA *  F  = new CARI_OTO_EKSTRE_ARAMA( p_cari_hesap_id, parent);
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   CARI_OTO_EKSTRE_ARAMA::CARI_OTO_EKSTRE_ARAMA
***************************************************************************************/

CARI_OTO_EKSTRE_ARAMA::CARI_OTO_EKSTRE_ARAMA ( int p_cari_hesap_id, QWidget * parent ) : ARAMA_KERNEL ( parent ), m_ui ( new Ui::CARI_OTO_EKSTRE_ARAMA )
{
    m_cari_hesap_id = p_cari_hesap_id;

    m_ui->setupUi      ( this );
    START_ARAMA_KERNEL ( this, DB );
}

/**************************************************************************************
                   CARI_OTO_EKSTRE_ARAMA::SETUP_FORM ()
***************************************************************************************/

void CARI_OTO_EKSTRE_ARAMA::SETUP_FORM ()
{

    REGISTER_TABLE_WIDGET ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    m_ui->adakDate_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
    m_ui->adakDate_bts_tarihi->SET_DATE( MALI_ARRAY_LAST_DATE() );

    SET_HEADERS ( QStringList() << tr("base_fis_id") << tr("") << tr("") << tr("") << tr("")<< tr("") << tr("")
                                << tr("Fiş Tarihi") << tr("Fiş No")  <<tr("Açıklama") << tr("Borç") << tr("Alacak"));

    SET_SORTING ( false );

    SET_FIRST_FOCUS_WIDGET ( m_ui->adakDate_bas_tarihi );

    SET_PAGE_TITLE    (tr("CARİ EKSTRESİ"));

    SET_HELP_PAGE     ("cari_ekstresi");

    m_ui->tableWidget->hideColumn( BASE_FIS_ID_COLUMN );
    m_ui->tableWidget->hideColumn( MODUL_ID_COLUMN );
    m_ui->tableWidget->hideColumn( FIS_TURU_COLUMN );
    m_ui->tableWidget->hideColumn( IADE_FATURASI_MI_COLUMN );
    m_ui->tableWidget->hideColumn( BORDRO_ISLEM_TURU_COLUMN );
    m_ui->tableWidget->hideColumn( FIS_ID_COLUMN );
    m_ui->tableWidget->hideColumn( SMM_MAKBUZ_TURU_COLUMN );

    m_ui->tableWidget->setColumnWidth( FIS_TARIHI_COLUMN    , 100 );
    m_ui->tableWidget->setColumnWidth( FIS_NO_COLUMN        , 40  );
    m_ui->tableWidget->setColumnWidth( ACIKLAMA_COLUMN      , 200 );
    m_ui->tableWidget->setColumnWidth( TOPLAM_BORC_COLUMN   , 80  );
    m_ui->tableWidget->setColumnWidth( TOPLAM_ALACAK_COLUMN , 80  );

    SET_SETTING_NAME  ("CARI_OTO_EKSTRE_ARAMA");

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );

    SET_FIND_BUTTON_NAME ( "Bul" );

    AUTO_START();
}

/**************************************************************************************
                   CARI_OTO_EKSTRE_ARAMA::CHECK_VAR
***************************************************************************************/

int CARI_OTO_EKSTRE_ARAMA::CHECK_VAR ( QObject * object )
{
    Q_UNUSED( object );
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_OTO_EKSTRE_ARAMA::CHECK_RUN
***************************************************************************************/

int CARI_OTO_EKSTRE_ARAMA::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   CARI_OTO_EKSTRE_ARAMA::SEARCH
***************************************************************************************/

void CARI_OTO_EKSTRE_ARAMA::SEARCH ()
{

    QList< int > fis_id_listesi;
    QList< int > irsaliyeli_fatura_fis_id_listesi; // irsaliye id tutar diger irsaliyelerden silinecek

    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "car_fisler, car_fis_satirlari", "car_fisler.fis_id, fis_tarihi",
                          "car_fis_satirlari.fis_id = car_fisler.fis_id  "
                          "AND ( car_fisler.cari_hesap_id = :cari_hesap_id_1 OR car_fis_satirlari.cari_hesap_id = :cari_hesap_id_2 )" );
    query.SET_VALUE     ( ":cari_hesap_id_1", m_cari_hesap_id );
    query.SET_VALUE     ( ":cari_hesap_id_2", m_cari_hesap_id );

    if( m_ui->checkBox_tarih_araligi->isChecked() EQ true ) {
        query.AND_EKLE ("fis_tarihi BETWEEN :fis_tarihi_1 AND :fis_tarihi_2");
        query.SET_VALUE(":fis_tarihi_1" , m_ui->adakDate_bas_tarihi->DATE() );
        query.SET_VALUE(":fis_tarihi_2" , m_ui->adakDate_bts_tarihi->DATE() );
    }


    if ( query.SELECT( "fis_tarihi ASC ") NE 0 ) {
        while( query.NEXT() EQ true ) {
            if( fis_id_listesi.contains( query.VALUE( 0 ).toInt() ) EQ false ) {
                fis_id_listesi << query.VALUE( 0 ).toInt();
            }
        }
    }

    if( fis_id_listesi.size() EQ 0 ) {
        return;
    }

    CARI_FIS_STRUCT CARI_FIS;
    int current_row = 0;
    for (int i = 0; i < fis_id_listesi.size(); ++i) {

        int cari_fis_id = fis_id_listesi.at(i);

        CARI_CLEAR_FIS_STRUCT( &CARI_FIS );

        CARI_FIS_BILGILERINI_OKU( &CARI_FIS, cari_fis_id );


        int fis_turu = 0;
        int iade_faturasi_mi = 0;
        int bordro_islem_turu = 0;
        int smm_makbuz_turu = 0;

        QString aciklama;

        switch( CARI_FIS.modul_id ) {

            case FATURA_MODULU: {

                FATURA_FISI_STRUCT FAT_FIS;
                FAT_FIS.KDV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
                FAT_FIS.KDV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
                FAT_FIS.KDV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];

                FAT_FIS.OTV_ORANLARI_ARRAY                =   new double  [KDV_OTV_ORANLARI_SAYISI];
                FAT_FIS.OTV_HESAPLARI_ID_ARRAY            =   new int     [KDV_OTV_ORANLARI_SAYISI];
                FAT_FIS.OTV_ORANINA_GORE_TUTARLAR_ARRAY   =   new double  [KDV_OTV_ORANLARI_SAYISI];
                FAT_FIS.TEVKIFATLI_KDV_HESAPLARI_ID_ARRAY =   new int     [KDV_OTV_ORANLARI_SAYISI];

                FATURA_CLEAR_FIS_STRUCT( &FAT_FIS );
                FATURA_FIS_BILGILERINI_OKU( &FAT_FIS, CARI_FIS.base_fis_id );

                fis_turu = FAT_FIS.fatura_turu;
                iade_faturasi_mi = FAT_FIS.iade_faturasi_mi;
                bordro_islem_turu = -1;

                aciklama.clear();

                aciklama.append( FAT_GET_FATURA_TURLERI_STRING( FAT_FIS.fatura_turu ) );

                if( FAT_FIS.fatura_turu EQ ENUM_FAT_IRSALIYELI_ALIS_FATURASI OR
                    FAT_FIS.fatura_turu EQ ENUM_FAT_IRSALIYELI_SATIS_FATURASI ) {
                    int irsaliye_id = STK_GET_FAT_IRS_BAGLANTISI_IRSALIYE_ID( CARI_FIS.base_fis_id );
                    irsaliyeli_fatura_fis_id_listesi << irsaliye_id;
                }

                break;
            }
            case CEKSENET_MODULU: {
                CEK_BORDRO_STRUCT CEK_FIS;
                CEK_BORDRO_BILGILERI_OKU( &CEK_FIS, CARI_FIS.base_fis_id );

                fis_turu = CEK_FIS.bordro_turu;
                iade_faturasi_mi = -1;
                bordro_islem_turu = CEK_FIS.bordro_islem_turu;

                aciklama.clear();
                aciklama.append( CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING( CEK_FIS.bordro_turu )) ;

                break;
            }
            case BANKA_MODULU: {
                BNK_DEFTER_FIS_STRUCT  BNK_FIS;
                BNK_DEFTER_FIS_BILGILERINI_OKU( &BNK_FIS, CARI_FIS.base_fis_id );

                fis_turu          = BNK_FIS.base_fis_turu;
                iade_faturasi_mi  = -1;
                bordro_islem_turu = -1;
                // makro, hareket ve virman fislerinin id ogrenmek icin kullanildi
                cari_fis_id       = BNK_FIS.base_fis_id;

                aciklama.clear();
                aciklama.append( "BANKA - " + BNK_GET_FIS_TURLERI_STRING( BNK_FIS.base_fis_turu ) );
                break;
            }
            case CARI_MODULU: {
                fis_turu = CARI_FIS.fis_turu;
                iade_faturasi_mi = -1;
                bordro_islem_turu = -1;

                aciklama.clear();
                QString cari_fis_turu = CARI_GET_FIS_TURLERI_STRING( CARI_FIS.fis_turu);
                if ( cari_fis_turu.contains( "Cari") EQ false ) {
                    cari_fis_turu.prepend( "CARI - ");
                }
                aciklama.append( cari_fis_turu );
                break;
            }
            case PERSONEL_MODULU: {
                aciklama.clear();
                aciklama.append( PRS_GET_BORDRO_DONEMI( CARI_FIS.base_fis_id ) + " BORDROSU");
                break;
            }
            case PERAKENDE_MODULU: {
                break;
            }
            case SMM_MODULU: {
                SMM_MAKBUZ_STRUCT SMM_FORMU;
                SMM_MAKBUZ_BILGILERINI_OKU( &SMM_FORMU, CARI_FIS.base_fis_id );

                smm_makbuz_turu = SMM_FORMU.makbuz_turu;
                aciklama.clear();
                aciklama.append( "Serbest Meslek Makbuzu" );
                break;
            }

            default:
                break;
        }

        current_row = ADD_NEW_ROW();

        m_ui->tableWidget->item( current_row, BASE_FIS_ID_COLUMN )->setText( QString::number( CARI_FIS.base_fis_id ) );
        m_ui->tableWidget->item( current_row, MODUL_ID_COLUMN )->setText( QString::number( CARI_FIS.modul_id ) );
        m_ui->tableWidget->item( current_row, FIS_TURU_COLUMN )->setText( QString::number( fis_turu ) );
        m_ui->tableWidget->item( current_row, IADE_FATURASI_MI_COLUMN )->setText( QString::number( iade_faturasi_mi) );
        m_ui->tableWidget->item( current_row, BORDRO_ISLEM_TURU_COLUMN )->setText( QString::number( bordro_islem_turu ) );
        m_ui->tableWidget->item( current_row, SMM_MAKBUZ_TURU_COLUMN )->setText( QString::number( smm_makbuz_turu ) );
        m_ui->tableWidget->item( current_row, FIS_ID_COLUMN )->setText( QString::number( cari_fis_id ) );
        m_ui->tableWidget->item( current_row, FIS_TARIHI_COLUMN )->setText( QDate::fromString( CARI_FIS.fis_tarihi, "yyyy.MM.dd" ).toString( "dd MMMM yyyy") );
        m_ui->tableWidget->item( current_row, FIS_NO_COLUMN  )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
        m_ui->tableWidget->item( current_row, FIS_NO_COLUMN )->setText( QString::number( CARI_FIS.fis_no ) );
        m_ui->tableWidget->item( current_row, ACIKLAMA_COLUMN )->setText( aciklama );
        m_ui->tableWidget->item( current_row, TOPLAM_BORC_COLUMN )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
        m_ui->tableWidget->item( current_row, TOPLAM_ALACAK_COLUMN )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
        m_ui->tableWidget->item( current_row, TOPLAM_BORC_COLUMN )->setText( VIRGUL_EKLE( QVariant( CARI_FIS.toplam_borc ).toString(), 2 ) );
        m_ui->tableWidget->item( current_row, TOPLAM_ALACAK_COLUMN )->setText( VIRGUL_EKLE( QVariant( CARI_FIS.toplam_alacak ).toString(), 2 ) );
    }

    QList <int> irsaliye_list = IRS_GET_IRSALIYE_ID_LIST( m_cari_hesap_id);

    // irsaliyeli faturalar siliniyor.
    for (int i = 0; i < irsaliyeli_fatura_fis_id_listesi.size(); ++i) {
        irsaliye_list.removeOne( irsaliyeli_fatura_fis_id_listesi.at(i));
    }

    STK_IRS_FIS_STRUCT IRS_FIS;
    for (int i = 0; i < irsaliye_list.size(); ++i) {

        STK_IRS_CLEAR_FIS_STRUCT( &IRS_FIS );

        STK_IRS_FIS_BILGILERINI_OKU( irsaliye_list.at(i), &IRS_FIS );

        current_row = ADD_NEW_ROW();

        m_ui->tableWidget->item( current_row, MODUL_ID_COLUMN )->setText( QString::number( IRSALIYE_MODULU) );
        m_ui->tableWidget->item( current_row, FIS_TURU_COLUMN )->setText( QString::number( IRS_FIS.fis_turu ) );
        m_ui->tableWidget->item( current_row, IADE_FATURASI_MI_COLUMN )->setText( QString::number( IRS_FIS.iade_irsaliyesi_mi) );
        m_ui->tableWidget->item( current_row, FIS_ID_COLUMN )->setText( QString::number( irsaliye_list.at(i) ) );
        m_ui->tableWidget->item( current_row, FIS_TARIHI_COLUMN )->setText( QDate::fromString( IRS_FIS.fis_tarihi, "yyyy.MM.dd" ).toString( "dd MMMM yyyy") );
        m_ui->tableWidget->item( current_row, FIS_NO_COLUMN  )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
        m_ui->tableWidget->item( current_row, FIS_NO_COLUMN )->setText( QString::number( IRS_FIS.fis_no ) );
        m_ui->tableWidget->item( current_row, ACIKLAMA_COLUMN )->setText( STK_GET_STOK_FIS_TURU( IRS_FIS.fis_turu ) );
        m_ui->tableWidget->item( current_row, TOPLAM_BORC_COLUMN )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
        m_ui->tableWidget->item( current_row, TOPLAM_BORC_COLUMN )->setText( VIRGUL_EKLE( QVariant( IRS_FIS.irsaliye_tutari ).toString(), 2 ) );
    }


    //! QTABLEWIDGET COLONA GORE SIRALAMA
    m_ui->tableWidget->sortItems( FIS_TARIHI_COLUMN, Qt::AscendingOrder );
}

/**************************************************************************************
                        CARI_OTO_EKSTRE_ARAMA::SELECTED_LINE
***************************************************************************************/

void CARI_OTO_EKSTRE_ARAMA::SELECTED_LINE ( int selected_row_number )
{
    Q_UNUSED( selected_row_number );
}

/**************************************************************************************
                   CARI_OTO_EKSTRE_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int CARI_OTO_EKSTRE_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    // Buraya kullanici bir sonucu double click yaptiginda duser ve donus degerine gore surec devam eder.
   int cari_fis_id          = m_ui->tableWidget->item ( selected_row_number, FIS_ID_COLUMN )->text().toInt();
   int base_fis_id          = m_ui->tableWidget->item ( selected_row_number, BASE_FIS_ID_COLUMN )->text().toInt();
   int modul_id             = m_ui->tableWidget->item ( selected_row_number, MODUL_ID_COLUMN )->text().toInt();
   int fis_turu             = m_ui->tableWidget->item ( selected_row_number, FIS_TURU_COLUMN )->text().toInt();
   int iade_faturasi_mi     = m_ui->tableWidget->item ( selected_row_number, IADE_FATURASI_MI_COLUMN )->text().toInt();
   int bordro_islem_turu    = m_ui->tableWidget->item ( selected_row_number, BORDRO_ISLEM_TURU_COLUMN )->text().toInt();
   int smm_makbuz_turu      = m_ui->tableWidget->item ( selected_row_number, SMM_MAKBUZ_TURU_COLUMN )->text().toInt();


   switch( modul_id ) {

       case BANKA_MODULU:
           //! buraya gelmeden once cari fis id alanina banka idleri setlendi
           //! burda kullanilabilir
           if( fis_turu EQ ENUM_BANKA_HAREKET_FISI ) {
                OPEN_BNK_HAREKET_FISI( fis_turu, cari_fis_id, modul_id, nativeParentWidget() );
           }
           else if ( fis_turu EQ ENUM_BANKA_MAKRO_FISI ) {
                OPEN_BNK_MAKRO_FISI( cari_fis_id, modul_id, nativeParentWidget() );
           }
           else {
               OPEN_BNK_VIRMAN_FORMU( cari_fis_id, nativeParentWidget() );
           }
           break;
       case CARI_MODULU:
           if ( fis_turu EQ ENUM_CARI_TAHSILAT_FISI ) {
                OPEN_CARI_TAHSILAT_FISI( cari_fis_id, nativeParentWidget() );
           }
           else if ( fis_turu EQ ENUM_CARI_ODEME_FISI ) {
                OPEN_CARI_ODEME_FISI( cari_fis_id, modul_id, nativeParentWidget() );
           }
           else if ( fis_turu EQ ENUM_CARI_VIRMAN_FORMU ) {
                OPEN_CARI_VIRMAN_FORMU( cari_fis_id, nativeParentWidget() );
           }
           else {
                OPEN_CARI_HAREKET_FISI( cari_fis_id, fis_turu, modul_id, nativeParentWidget() );
           }
           break;
       case CEKSENET_MODULU:
           OPEN_CEK_SENET_BORDROSU_FISI ( base_fis_id, modul_id, bordro_islem_turu , nativeParentWidget(), fis_turu );
           break;
       case FATURA_MODULU:
           OPEN_FATURA_FISI( fis_turu, base_fis_id, iade_faturasi_mi, nativeParentWidget());
           break;
       case SMM_MODULU:
           OPEN_SMM_MAKBUZ_FORMU( base_fis_id, smm_makbuz_turu, nativeParentWidget() );
           break;
       case PERSONEL_MODULU:
           SHOW_BORDRO_INCELEME_TREE_SECIMI( nativeParentWidget() );
           break;

       case IRSALIYE_MODULU:
       case STOK_MODULU    :
           OPEN_STOK_FISI( cari_fis_id, ENUM_IRSALIYE, fis_turu, iade_faturasi_mi, nativeParentWidget() );
           break;

       default:
           break;
   }

    return ADAK_CONTINUE;
}
