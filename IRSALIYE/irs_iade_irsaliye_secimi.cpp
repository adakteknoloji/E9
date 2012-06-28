#include <QTableWidget>
#include <QDate>
#include "adak_sql.h"
 
#include "irs_iade_irsaliye_secimi_class.h"
#include "irs_iade_irsaliye_secimi_open.h"
#include "secim_kerneli.h"
#include "stok_enum.h"
 

extern ADAK_SQL *           DB;

#define FIS_ID_COLUMN                       0
#define FIS_NO_COLUMN                       1
#define IRSALIYE_BELGE_SERI_NO_COLUMN       2
#define IRSALIYE_TARIHI_COLUMN              3



/**************************************************************************************
                   OPEN_IRS_IADE_IRSALIYE_SECIMI
***************************************************************************************/

int OPEN_IRS_IADE_IRSALIYE_SECIMI ( QString p_irsaliye_tarihi, int p_cari_hesap_id, int p_irsaliye_alis_satis_turu, int p_irsaliye_turu, QWidget *   p_parent )
{
    int secilen_id = -1;
    IRS_IADE_IRSALIYE_SECIMI * F = new IRS_IADE_IRSALIYE_SECIMI ( p_irsaliye_tarihi, p_cari_hesap_id, p_irsaliye_alis_satis_turu, p_irsaliye_turu, p_parent );
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return ( secilen_id );
}

/**************************************************************************************
                   IRS_IADE_IRSALIYE_SECIMI::IRS_IADE_IRSALIYE_SECIMI
***************************************************************************************/

IRS_IADE_IRSALIYE_SECIMI::IRS_IADE_IRSALIYE_SECIMI ( QString p_irsaliye_tarihi, int p_cari_hesap_id, int p_irsaliye_alis_satis_turu,int p_irsaliye_turu, QWidget * p_parent ) : SECIM_KERNELI ( p_parent )
{
    SET_HELP_PAGE ("irs_iade_irsaliye_secimi.html");

    m_irsaliye_tarihi_bitis     = p_irsaliye_tarihi;
    m_cari_hesap_id             = p_cari_hesap_id;
    m_irsaliye_alis_satis_turu  = p_irsaliye_alis_satis_turu;
    m_irsaliye_turu             = p_irsaliye_turu;


    SET_WINDOW_SIZE     ( 300, 500 );
    SET_SORTING         ( false );
    SET_HEADERS         ( QStringList() << tr("irsaliye_id") << tr("İrsaliye No") <<tr("İrs. Seri No")
                                        << tr("İrs. Tarihi") );

    INIT_KERNEL         ( DB );

    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    tableWidget->setColumnWidth ( FIS_NO_COLUMN,                    100 );
    tableWidget->setColumnWidth ( IRSALIYE_BELGE_SERI_NO_COLUMN,    100 );
    tableWidget->setColumnWidth ( IRSALIYE_TARIHI_COLUMN,           100 );

    SET_SETTING_NAME            ( "IRS_IADE_IRSALIYE_SECIMI" );

    SET_PAGE_TITLE              ( tr ( "İRS - İADE İRSALİYE SEÇİMİ" ) );

    tableWidget->hideColumn     ( 0 );

}

/**************************************************************************************
                   IRS_IADE_IRSALIYE_SECIMI::FILL_TABLE
***************************************************************************************/

void IRS_IADE_IRSALIYE_SECIMI::FILL_TABLE()
{
    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    SQL_QUERY f_query ( DB );
    SQL_QUERY s_query ( DB );

    f_query.PREPARE_SELECT("stk_fisler" , "fis_id, fis_no, belge_seri, belge_numarasi, fis_tarihi",
                           "cari_hesap_id                = :cari_hesap_id "
                           "AND   irsaliye_alis_satis_turu     = :irsaliye_alis_satis_turu "
                           "AND   hatali_irsaliye_mi           = :hatali_irsaliye_mi "
                           "AND   iade_irsaliyesi_mi           = :iade_irsaliyesi_mi " );

    f_query.SET_VALUE ( ":cari_hesap_id"            , m_cari_hesap_id             );
    f_query.SET_VALUE ( ":irsaliye_alis_satis_turu" , m_irsaliye_alis_satis_turu  );
    f_query.SET_VALUE ( ":hatali_irsaliye_mi"       , 0);
    f_query.SET_VALUE ( ":iade_irsaliyesi_mi"       , 0);


    switch( m_irsaliye_turu ) {

        case ENUM_IRS_HAKEDIS_ALIS :
        case ENUM_IRS_HAKEDIS_SATIS: {

            int fis_turu = ENUM_IRS_HAKEDIS_ALIS;
            if( m_irsaliye_turu EQ ENUM_IRS_HAKEDIS_ALIS ) {
                fis_turu = ENUM_IRS_HAKEDIS_SATIS;
            }

            f_query.AND_EKLE ( "fis_turu = :fis_turu");
            f_query.SET_VALUE( ":fis_turu", fis_turu );

            break;
        }
        case ENUM_IRS_KONSINYE_GIRIS:
        case ENUM_IRS_KONSINYE_CIKIS: {

            int fis_turu = ENUM_IRS_KONSINYE_GIRIS;
            if( m_irsaliye_turu EQ ENUM_IRS_KONSINYE_GIRIS ) {
                fis_turu = ENUM_IRS_KONSINYE_CIKIS;
            }

            f_query.AND_EKLE ( "fis_turu = :fis_turu");
            f_query.SET_VALUE( ":fis_turu", fis_turu );

            break;
        }
        default : {
            //! HAKEDIS VE KONSINYE FATURALARI AYRI GOSTERILDIGI ICIN TEKRAR
            //! DIGER IRSALIYELER  ILE GORUNMEMESI GEREKIR.
            f_query.AND_EKLE ( "fis_turu != :fis_turu_1 ");
            f_query.SET_VALUE( ":fis_turu_1", ENUM_IRS_HAKEDIS_SATIS );
            f_query.AND_EKLE ( "fis_turu != :fis_turu_2");
            f_query.SET_VALUE( ":fis_turu_2", ENUM_IRS_HAKEDIS_ALIS );
            f_query.AND_EKLE ( "fis_turu != :fis_turu_3");
            f_query.SET_VALUE( ":fis_turu_3", ENUM_IRS_KONSINYE_GIRIS );
            f_query.AND_EKLE ( "fis_turu != :fis_turu_4");
            f_query.SET_VALUE( ":fis_turu_4", ENUM_IRS_KONSINYE_CIKIS );

            break;
        }
    }


    if ( f_query.SELECT("fis_tarihi ASC,fis_no ASC") EQ 0 ) {
        return;
    }

    int current_row = 0;

    while ( f_query.NEXT() EQ true ) {

        int irsaliye_id = f_query.VALUE(0).toInt();

        bool irsaliye_listeye_eklensin_mi = false;

        s_query.PREPARE_SELECT ( "stk_fis_satirlari ","satirdaki_urun_miktari ,faturalanan_miktar,iade_edilen_miktar ",
                                 "fis_id = :fis_id ");
        s_query.SET_VALUE      ( ":fis_id", irsaliye_id );

        if ( s_query.SELECT() EQ 0 ) {
            continue;
        }
        while (s_query.NEXT()) {

            double satirdaki_urun_miktari = s_query.VALUE(0).toDouble();
            double faturalanan_miktar     = s_query.VALUE(1).toDouble();
            double iade_edilen_miktar     = s_query.VALUE(2).toDouble();

            if ( (satirdaki_urun_miktari - faturalanan_miktar - iade_edilen_miktar) <= 0 ) {
                continue;
            }

            irsaliye_listeye_eklensin_mi = true;
        }

        if ( irsaliye_listeye_eklensin_mi EQ true ) {
            current_row = ADD_NEW_ROW ();

            tableWidget->item ( current_row, FIS_ID_COLUMN                  )->setText ( f_query.VALUE(0).toString() );
            tableWidget->item ( current_row, FIS_NO_COLUMN                  )->setText ( f_query.VALUE(1).toString() );
            tableWidget->item ( current_row, IRSALIYE_BELGE_SERI_NO_COLUMN  )->setText ( f_query.VALUE(2).toString() + " " + f_query.VALUE(3).toString() );
            tableWidget->item ( current_row, IRSALIYE_TARIHI_COLUMN         )->setText ( QDate::fromString ( f_query.VALUE(4).toString(), "yyyy.MM.dd").toString("dd MMMM yyyy ") );
        }
    }
}

/**************************************************************************************
                   IRS_IADE_IRSALIYE_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int IRS_IADE_IRSALIYE_SECIMI::SINGLE_LINE_SELECTED(int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    *m_secilen_id = table_widget->item ( selected_row_number, FIS_ID_COLUMN )->text().toInt();
    return ADAK_EXIT;
}
