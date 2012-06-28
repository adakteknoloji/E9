#include <QTableWidget>
#include <QDate>
#include "adak_sql.h"
#include "fat_iade_fatura_secimi_class.h"
#include "fat_enum.h"

#define FIS_ID_COLUMN                     0
#define FATURA_BELGE_SERI_NO_COLUMN       1
#define FIS_NO_COLUMN                     2
#define FATURA_TARIHI_COLUMN              3
#define FATURA_TUTARI_COLUMN              4

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_IADE_FATURA_SECIMI
***************************************************************************************/

int OPEN_IADE_FATURA_SECIMI ( int p_cari_hesap_id, int p_fatura_alis_satis_iade_turu, int p_fatura_turu, QWidget *p_parent )
{
    int          secilen_id    = -1;
    FAT_IADE_FATURA_SECIMI * F =  new FAT_IADE_FATURA_SECIMI ( p_cari_hesap_id, p_fatura_alis_satis_iade_turu, p_fatura_turu, p_parent );
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return ( secilen_id );
}

/**************************************************************************************
                   FAT_IADE_FATURA_SECIMI::FAT_IADE_FATURA_SECIMI
***************************************************************************************/

FAT_IADE_FATURA_SECIMI::FAT_IADE_FATURA_SECIMI ( int p_cari_hesap_id, int  p_fatura_alis_satis_iade_turu, int p_fatura_turu, QWidget * p_parent ) : SECIM_KERNELI ( p_parent )
{
    m_fatura_turu                   = p_fatura_turu;
    m_cari_hesap_id                 = p_cari_hesap_id;
    m_fatura_alis_satis_iade_turu   = p_fatura_alis_satis_iade_turu;

    SET_WINDOW_SIZE     ( 300, 500 );
    SET_SORTING         ( false );
    SET_HEADERS         ( QStringList() << tr("fatura_id") << tr("Fat. Seri No") << tr("Fatura No")
                          << tr("Fat. Tarihi") << tr("Fat.Tutarı") );

    SET_PAGE_TITLE( tr ( "FAT - İADE FATURA SEÇİMİ" ) );

    SET_HELP_PAGE( tr("fat_iade_fatura_secimi.html") );



    INIT_KERNEL         ( DB );

    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    tableWidget->setColumnWidth ( FATURA_BELGE_SERI_NO_COLUMN,    100 );
    tableWidget->setColumnWidth ( FIS_NO_COLUMN,                  60 );
    tableWidget->setColumnWidth ( FATURA_TARIHI_COLUMN,           100 );
    tableWidget->setColumnWidth ( FATURA_TUTARI_COLUMN,           100 );

    SET_SETTING_NAME            ( "FATURA_IADE_SECIMI" );

    tableWidget->hideColumn     ( 0 );
}

/**************************************************************************************
                   FAT_IADE_FATURA_SECIMI::FILL_TABLE
***************************************************************************************/

void FAT_IADE_FATURA_SECIMI::FILL_TABLE()
{
    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    SQL_QUERY f_query ( DB );
    SQL_QUERY s_query ( DB );

    f_query.PREPARE_SELECT("fat_faturalar" , "fis_id, fis_no, belge_seri, belge_numarasi, fatura_tarihi, fatura_turu, fatura_tutari",
                           "cari_hesap_id                   = :cari_hesap_id "
                           "AND   fatura_alis_satis_turu    = :fatura_alis_satis_turu "
                           "AND   hatali_fatura_mi          = :hatali_fatura_mi "
                           "AND   iade_faturasi_mi          = :iade_faturasi_mi " );

    f_query.SET_VALUE ( ":cari_hesap_id"                    , m_cari_hesap_id               );
    f_query.SET_VALUE ( ":fatura_alis_satis_turu"           , m_fatura_alis_satis_iade_turu );
    f_query.SET_VALUE ( ":hatali_fatura_mi"                 , 0);
    f_query.SET_VALUE ( ":iade_faturasi_mi"                 , 0);

    switch( m_fatura_turu ) {

    case ENUM_FAT_HAKEDIS_ALIS_FATURASI :
    case ENUM_FAT_HAKEDIS_SATIS_FATURASI:
        f_query.AND_EKLE ( "fatura_turu = :fatura_turu" );
        if( m_fatura_turu EQ ENUM_FAT_HAKEDIS_ALIS_FATURASI ) {
            f_query.SET_VALUE( ":fatura_turu", ENUM_FAT_HAKEDIS_SATIS_FATURASI );
        }
        else {
            f_query.SET_VALUE( ":fatura_turu", ENUM_FAT_HAKEDIS_ALIS_FATURASI );
        }
        break;
    default:
        //! HAKEDIS FATURALARI DIGER FATURA TURLERIN ICINDE GORUNMEMESI GEREKIR.
        f_query.AND_EKLE ( "fatura_turu != :fatura_turu_1");
        f_query.SET_VALUE( ":fatura_turu_1", ENUM_FAT_HAKEDIS_SATIS_FATURASI );
        f_query.AND_EKLE ( "fatura_turu != :fatura_turu_2");
        f_query.SET_VALUE( ":fatura_turu_2", ENUM_FAT_HAKEDIS_ALIS_FATURASI );
        break;
    }


    if ( f_query.SELECT("fatura_tarihi ASC , fis_no ASC") EQ 0 ) {
        return;
    }

    int current_row = 0;

    while ( f_query.NEXT() EQ true ) {

        int fis_id   = f_query.VALUE(0).toInt();
        int fat_turu = f_query.VALUE(5).toInt();

        switch ( fat_turu ) {

        case ENUM_FAT_VADE_FARKI_ALIS_FATURASI  :
        case ENUM_FAT_VADE_FARKI_SATIS_FATURASI :
        case ENUM_FAT_HIZMET_SATIS_FATURASI     :
        case ENUM_FAT_HIZMET_ALIS_FATURASI      :
            //! VADE FARKI VE HIZMET FATURASINDA IADE ISLEMI
            //! YAPILMADIGI ICIN LISTEDE GOSTERMEMELIYIZ
            continue;
            break;

        default:
            break;
        }

        s_query.PREPARE_SELECT ( "fat_fatura_satirlari ","satirdaki_urun_miktari , iade_edilen_miktar",
                                 "fis_id = :fis_id ");

        s_query.SET_VALUE      ( ":fis_id", fis_id );

        if ( s_query.SELECT() EQ 0 ) {
            continue;
        }
        s_query.NEXT();
        double satirdaki_urun_miktari = s_query.VALUE(0).toDouble();
        double iade_edilen_miktar     = s_query.VALUE(1).toDouble();

        if ( (satirdaki_urun_miktari - iade_edilen_miktar) <= 0 ) {
            continue;
        }

        current_row = ADD_NEW_ROW ();

        tableWidget->item ( current_row, FIS_ID_COLUMN               )->setText ( f_query.VALUE(0).toString() );
        tableWidget->item ( current_row, FIS_NO_COLUMN               )->setText ( f_query.VALUE(1).toString() );
        tableWidget->item ( current_row, FATURA_BELGE_SERI_NO_COLUMN )->setText ( f_query.VALUE(2).toString() + " " + f_query.VALUE(3).toString() );
        tableWidget->item ( current_row, FATURA_TARIHI_COLUMN        )->setText ( QDate::fromString ( f_query.VALUE(4).toString(), "yyyy.MM.dd").toString("dd MMMM yyyy ") );
        tableWidget->item ( current_row, FATURA_TUTARI_COLUMN        )->setTextAlignment( Qt::AlignRight + Qt::AlignVCenter );
        tableWidget->item ( current_row, FATURA_TUTARI_COLUMN        )->setText ( VIRGUL_EKLE( f_query.VALUE(6).toString() ) );

    }
}

/**************************************************************************************
                   FAT_IADE_FATURA_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int FAT_IADE_FATURA_SECIMI::SINGLE_LINE_SELECTED(int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    *m_secilen_id = table_widget->item ( selected_row_number, FIS_ID_COLUMN )->text().toInt();
    return ADAK_EXIT;
}
