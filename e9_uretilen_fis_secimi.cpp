#include <QDate>
#include <QTableWidget>
#include "adak_sql.h"
#include "e9_uretilen_fis_secimi_class.h"
#include "e9_uretilen_fis_secimi_open.h"
#include "secim_kerneli.h"
#include "bnk_fis_utils.h"
#include "cari_fis_utils.h"
#include "muh_fis_utils.h"
#include "cek_fis_utils.h"
#include "cari_hareket_fisi_open.h"
#include "bnk_hareket_fisi_open.h"
#include "muh_muhasebe_fisi_open.h"
#include "cek_senet_bordrosu_fisi_open.h"
#include "cari_console_utils.h"
#include "muh_console_utils.h"
#include "bnk_console_utils.h"
#include "cek_console_utils.h"

extern ADAK_SQL *           DB;

#define ROW_ID_COLUMN                  0
#define FIS_ID_COLUMN                  1
#define FIS_TURU_COLUMN                2
#define FIS_TURU_ADI_COLUMN            3
#define FIS_NO_COLUMN                  4
#define FIS_HESABA_GIREN_COLUMN        5
#define FIS_HESAPTAN_CIKAN_COLUMN      6
#define FIS_ACIKLAMA_COLUMN            7


/**************************************************************************************
                   E9_URETILEN_FIS_SEC
***************************************************************************************/

void E9_URETILEN_FIS_SEC ( int p_program_id, int p_modul_id, int p_fis_id, QWidget * parent )
{
    E9_URETILEN_FIS_SECIMI * F = new E9_URETILEN_FIS_SECIMI ( p_program_id, p_modul_id, p_fis_id, parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   E9_URETILEN_FIS_SECIMI::E9_URETILEN_FIS_SECIMI
***************************************************************************************/

E9_URETILEN_FIS_SECIMI::E9_URETILEN_FIS_SECIMI( int p_program_id, int p_modul_id, int p_fis_id, QWidget * parent ) : SECIM_KERNELI ( parent )
{
    m_program_id = p_program_id;
    m_modul_id   = p_modul_id;
    m_fis_id     = p_fis_id;

    SET_WINDOW_SIZE( 300, 800 );

    SET_HEADERS (QStringList() << "row_id" << "Fis Id" << "fis turu" << "Fis Turu adi" << "Fis No" << "Borç" << "Alacak"  << "Açıklama");

    SET_PAGE_TITLE    ( tr ( "ÜRETİLEN FİŞLER" ) );
    SET_SETTING_NAME  ( "E9_URETILEN_FIS_SECIMI" );
    SET_HELP_PAGE     ( tr("e9_uretilen_fisler") );

    INIT_KERNEL ( DB );

    QTableWidget * table_widget = GET_TABLE_WIDGET();

    table_widget->hideColumn( ROW_ID_COLUMN );      // row_id
    table_widget->hideColumn( FIS_ID_COLUMN );      // fis_id
    table_widget->hideColumn( FIS_TURU_COLUMN );    // fis_id
    table_widget->setColumnWidth( FIS_TURU_ADI_COLUMN      , 140 );
    table_widget->setColumnWidth( FIS_NO_COLUMN            , 40  );
    table_widget->setColumnWidth( FIS_HESABA_GIREN_COLUMN  , 80  );
    table_widget->setColumnWidth( FIS_HESAPTAN_CIKAN_COLUMN, 80  );
    table_widget->setColumnWidth( FIS_ACIKLAMA_COLUMN      , 300 );

}

/**************************************************************************************
                   E9_URETILEN_FIS_SECIMI::FILL_TABLE
***************************************************************************************/

void E9_URETILEN_FIS_SECIMI::FILL_TABLE()
{

      switch( m_modul_id ) {

          case BANKA_MODULU:
              CARI_ENT_OLMUS_MU();
              MUH_ENT_OLMUS_MU();
              CEKSENET_ENT_OLMUS_MU();
              break;

          case CARI_MODULU:
              BNK_ENT_OLMUS_MU();
              MUH_ENT_OLMUS_MU();
              CEKSENET_ENT_OLMUS_MU();
              break;

          case CEKSENET_MODULU:
              BNK_ENT_OLMUS_MU();
              CARI_ENT_OLMUS_MU();
              MUH_ENT_OLMUS_MU();
              break;
          case IRSALIYE_MODULU:
          case ISLETME_MODULU:
          case SMM_MODULU:
          case STOK_MODULU:
          case DEMIRBAS_MODULU:
          case FATURA_MODULU:
              BNK_ENT_OLMUS_MU();
              CARI_ENT_OLMUS_MU();
              MUH_ENT_OLMUS_MU();
              CEKSENET_ENT_OLMUS_MU();
              break;
          default:
              break;
      }

    QTableWidget * table_widget = GET_TABLE_WIDGET();

    int current_row = 0;

    QStringList fis_bilgileri;

    for (int i = 0; i < tum_ent_fis_listesi.size(); ++i) {

        current_row = ADD_NEW_ROW();

        fis_bilgileri = tum_ent_fis_listesi.at( i );

        table_widget->item(current_row, ROW_ID_COLUMN )->setText ( QString::number( i ) );
        table_widget->item(current_row, FIS_ID_COLUMN )->setText ( fis_bilgileri.at(2) );
        table_widget->item(current_row, FIS_NO_COLUMN )->setText ( fis_bilgileri.at(3) );
        table_widget->item(current_row, FIS_ACIKLAMA_COLUMN )->setTextAlignment( Qt::AlignLeft + Qt::AlignVCenter );
        table_widget->item(current_row, FIS_ACIKLAMA_COLUMN )->setText ( fis_bilgileri.at(4));
        table_widget->item(current_row, FIS_HESABA_GIREN_COLUMN )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
        table_widget->item(current_row, FIS_HESABA_GIREN_COLUMN )->setText ( fis_bilgileri.at(5));
        table_widget->item(current_row, FIS_HESAPTAN_CIKAN_COLUMN )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
        table_widget->item(current_row, FIS_HESAPTAN_CIKAN_COLUMN )->setText ( fis_bilgileri.at(6));
        table_widget->item(current_row, FIS_TURU_ADI_COLUMN )->setText ( fis_bilgileri.at(7));
        table_widget->item(current_row, FIS_TURU_COLUMN )->setText ( fis_bilgileri.at(8));

    }
}

/**************************************************************************************
                   E9_URETILEN_FIS_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int E9_URETILEN_FIS_SECIMI::SINGLE_LINE_SELECTED(int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    int secilen_id = table_widget->item ( selected_row_number, 0 )->text().toInt();

    QStringList fis_bilgileri;

    fis_bilgileri = tum_ent_fis_listesi.at( secilen_id );

    int modul_id = fis_bilgileri.at( 1 ).toInt();
    int fis_id   = fis_bilgileri.at( 2 ).toInt();
    int ent_fisinin_oldugu_modul = fis_bilgileri.at( 8 ).toInt();
    int fis_turu = fis_bilgileri.at( 9 ).toInt();

    switch( ent_fisinin_oldugu_modul ) {

        case BANKA_MODULU:
            OPEN_BNK_HAREKET_FISI( fis_turu, fis_id, modul_id, nativeParentWidget() );
            break;
        case CARI_MODULU:
            OPEN_CARI_HAREKET_FISI( fis_id, fis_turu, modul_id, nativeParentWidget() );
            break;
        case MUHASEBE_MODULU:
            OPEN_MUHASEBE_FISI( fis_id, fis_turu, modul_id, nativeParentWidget() );
            break;
        case CEKSENET_MODULU:
            OPEN_CEK_SENET_BORDROSU_FISI ( fis_id, modul_id, fis_bilgileri.at(10).toInt() , nativeParentWidget(), fis_turu );
            break;
        default:
            break;
    }

    return ADAK_CONTINUE;
}

/**************************************************************************************
                   E9_URETILEN_FIS_SECIMI::CARI_FILL_FIS
***************************************************************************************/

void E9_URETILEN_FIS_SECIMI::CARI_ENT_OLMUS_MU()
{
    int fis_id = CARI_GET_ENT_FIS_ID( m_program_id, m_modul_id, m_fis_id );

    if( fis_id EQ 0 ) {
        return;
    }

    CARI_FIS_STRUCT CARI_FIS;

    CARI_CLEAR_FIS_STRUCT( &CARI_FIS );

    CARI_FIS_BILGILERINI_OKU( &CARI_FIS, fis_id );

    QStringList fis_bilgileri;

    fis_bilgileri << QString::number( CARI_FIS.program_id )
                  << QString::number( CARI_FIS.modul_id )
                  << QString::number( fis_id )
                  << QString::number( CARI_FIS.fis_no )
                  << CARI_FIS.aciklama
                  << VIRGUL_EKLE( QVariant( CARI_FIS.toplam_borc  ).toString(), 2)
                  << VIRGUL_EKLE( QVariant( CARI_FIS.toplam_alacak ).toString(), 2)
                  << "CARİ - " + CARI_GET_FIS_TURLERI_STRING( CARI_FIS.fis_turu )
                  << QString::number( CARI_MODULU )
                  << QString::number( CARI_FIS.fis_turu );

    tum_ent_fis_listesi << fis_bilgileri;
}

/**************************************************************************************
                   E9_URETILEN_FIS_SECIMI::BNK_ENT_OLMUS_MU
***************************************************************************************/

void E9_URETILEN_FIS_SECIMI::BNK_ENT_OLMUS_MU()
{
    int fis_id = BNK_GET_ENT_FIS_ID( m_program_id, m_modul_id, m_fis_id );

    if( fis_id EQ 0 ) {
        return;
    }
    BNK_HAREKET_FIS_STRUCT BNK_FIS;

    BNK_CLEAR_HAREKET_FIS_STRUCT( &BNK_FIS );

    BNK_HAREKET_FIS_BILGILERINI_OKU( &BNK_FIS, fis_id );

    QStringList fis_bilgileri;

    fis_bilgileri << QString::number( BNK_FIS.program_id )
                  << QString::number( BNK_FIS.modul_id )
                  << QString::number( fis_id )
                  << QString::number( BNK_FIS.fis_no )
                  << BNK_FIS.aciklama
                  << VIRGUL_EKLE( QVariant( BNK_FIS.toplam_hesaba_giren ).toString(), 2 )
                  << VIRGUL_EKLE( QVariant( BNK_FIS.toplam_hesaptan_cikan ).toString(), 2 )
                  << "BANKA - " + BNK_GET_HESAP_TURU_STRING( BNK_FIS.fis_turu )
                  << QString::number( BANKA_MODULU )
                  << QString::number( BNK_FIS.fis_turu );

    tum_ent_fis_listesi << fis_bilgileri;
}

/**************************************************************************************
                   E9_URETILEN_FIS_SECIMI::MUH_ENT_OLMUS_MU
***************************************************************************************/

void E9_URETILEN_FIS_SECIMI::MUH_ENT_OLMUS_MU()
{
    int fis_id = MUH_GET_ENT_FIS_ID( m_program_id, m_modul_id, m_fis_id );

    if ( fis_id EQ 0 ) {
        return;
    }

    MUH_FIS_STRUCT MUH_FIS;

    MUH_CLEAR_FIS_STRUCT( &MUH_FIS );

    MUH_FIS_BILGILERINI_OKU( &MUH_FIS, fis_id );

    QStringList fis_bilgileri;

    fis_bilgileri << QString::number( MUH_FIS.program_id )
                  << QString::number( MUH_FIS.modul_id )
                  << QString::number( fis_id )
                  << QString::number( MUH_FIS.fis_no )
                  << MUH_FIS.aciklama
                  << VIRGUL_EKLE( QVariant( MUH_FIS.toplam_borc ).toString(), 2 )
                  << VIRGUL_EKLE( QVariant( MUH_FIS.toplam_alacak ).toString(), 2 )
                  << "MUHASEBE - " + MUH_GET_FIS_TURLERI_STRING( MUH_FIS.fis_turu )
                  << QString::number( MUHASEBE_MODULU )
                  << QString::number( MUH_FIS.fis_turu );

    tum_ent_fis_listesi << fis_bilgileri;
}

/**************************************************************************************
                   E9_URETILEN_FIS_SECIMI::CEKSENET_ENT_OLMUS_MU
***************************************************************************************/

void E9_URETILEN_FIS_SECIMI::CEKSENET_ENT_OLMUS_MU()
{
    QStringList fis_id_list = CEK_GET_ENT_FIS_ID_LIST( m_program_id, m_modul_id, m_fis_id );

    if ( fis_id_list.size() EQ 0 ) {
        return;
    }

    for (int i = 0; i < fis_id_list.size(); ++i) {
        CEK_BORDRO_STRUCT CEK_SENET_FIS;

        CEK_BORDRO_BILGILERI_OKU( &CEK_SENET_FIS, fis_id_list.at(i).toInt() );

        QStringList fis_bilgileri;

        fis_bilgileri << QString::number( CEK_SENET_FIS.program_id )  // 0
                      << QString::number( CEK_SENET_FIS.modul_id )    // 1
                      << fis_id_list.at(i)                            // 2
                      << QString::number( CEK_SENET_FIS.bordro_no )   // 3
                      << CEK_SENET_FIS.aciklama                       // 4
                      << VIRGUL_EKLE( QVariant( CEK_SENET_FIS.toplam_tutar ).toString(), 2 )// 5
                      << VIRGUL_EKLE( QVariant( CEK_SENET_FIS.toplam_tutar ).toString(), 2 )// 6
                      << "ÇEK SENET - " + CEK_GET_CEK_SENET_BORDRO_ISLEM_TURU_STRING( CEK_SENET_FIS.bordro_turu ) // 7
                      << QString::number( CEKSENET_MODULU )           // 8
                      << QString::number( CEK_SENET_FIS.bordro_turu ) // 9
                      << QString::number( CEK_SENET_FIS.bordro_islem_turu );// istisna fazla eklendi // 10

        tum_ent_fis_listesi << fis_bilgileri;

    }
}
