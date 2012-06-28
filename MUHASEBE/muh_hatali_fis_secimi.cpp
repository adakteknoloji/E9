#include <QTableWidget>
#include <QDate>
#include "adak_utils.h"
#include "muh_hatali_fis_secimi_class.h"
#include "muh_hatali_fis_secimi_open.h"
#include "muh_console_utils.h"
#include "muh_muhasebe_fisi_open.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   HATALI_FIS_SECIMI::HATALI_FIS_SECIMI
***************************************************************************************/

void OPEN_HATALI_FIS_SEC ( QWidget * parent )
{
    HATALI_FIS_SECIMI * F = new HATALI_FIS_SECIMI ( parent );
    F->EXEC( FULL_SCREEN );
}

#include <QHeaderView>

/**************************************************************************************
                   HATALI_FIS_SECIMI::HATALI_FIS_SECIMI
***************************************************************************************/

HATALI_FIS_SECIMI::HATALI_FIS_SECIMI ( QWidget *parent ) : SECIM_KERNELI ( parent )
{
    SET_HELP_PAGE    ( "muhasebe-raporlari" );

    SET_WINDOW_SIZE  ( 300, 800 );

    SET_PAGE_TITLE   ( tr ( "MUH - HATALI FİŞLER" ) );

    SET_HEADERS ( QStringList()<<tr ( "Fiş Id" )<<tr ( "Fiş No" )<<tr ( "Fiş Tarihi" )<<tr ( "Açıklama" )
                               <<tr ( "Toplam Borç" )<<tr ( "Toplam Alacak" )<<tr ( "Fiş Türü"));

    INIT_KERNEL ( DB );

    QTableWidget * table_widget = GET_TABLE_WIDGET();

    table_widget->horizontalHeader()->setDefaultSectionSize(0);

    table_widget->hideColumn(0);

    table_widget->horizontalHeader()->resizeSection ( 1, 10 );
    table_widget->horizontalHeader()->resizeSection ( 2, 20 );
    table_widget->horizontalHeader()->resizeSection ( 3, 20 );
    table_widget->horizontalHeader()->resizeSection ( 4, 20 );
    table_widget->horizontalHeader()->resizeSection ( 5, 20 );
    table_widget->horizontalHeader()->resizeSection ( 6, 10 );

    SET_SETTING_NAME             ( "MUH_HATALI_FIS_SECIMI" );

    SET_FIRST_FOCUS_WIDGET( table_widget );
}

/**************************************************************************************
                   HATALI_FIS_SECIMI::FILL_TABLE
***************************************************************************************/
void HATALI_FIS_SECIMI::FILL_TABLE()
{

    QTableWidget * table_widget = GET_TABLE_WIDGET();

    SQL_QUERY sql_query      ( DB );


    sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_id");
    if ( sql_query.SELECT() NE 0 ) {
        while ( sql_query.NEXT() ) {
            m_hesap_id_list << sql_query.VALUE(0).toString();
        }
    }


    sql_query.PREPARE_SELECT("muh_fisler" , "fis_id,fis_tarihi ,aciklama,toplam_borc,"
                             "toplam_alacak,fis_turu,modul_id ,fis_no" );

    if ( sql_query.SELECT("fis_no ASC") EQ 0 ) {
        return;
    }

    while ( sql_query.NEXT() EQ true ) {

        double toplam_borc   = sql_query.VALUE("toplam_borc").toDouble();
        double toplam_alacak = sql_query.VALUE("toplam_alacak").toDouble();
        int    fis_id        = sql_query.VALUE("fis_id").toInt();

        if ( toplam_borc EQ toplam_alacak  ) {
            //Fislerin satirlari kontrol edilecek.
            //Toplam borc ve alacak esit fakat satir toplamlari farkli ise
            //fis hatalidir.

            if ( toplam_borc NE 0 AND toplam_alacak NE 0 ) {
                if ( FIS_HATALI_MI( fis_id,toplam_borc,toplam_alacak ) EQ false ) {
                    continue;
                }
            }
        }
        int current_row = ADD_NEW_ROW();

        int fis_turu    = sql_query.VALUE("fis_turu").toInt();
        int modul_id    = sql_query.VALUE("modul_id").toInt();

        QString fis_turu_str;

        if ( modul_id EQ MUHASEBE_MODULU ) {
            fis_turu_str = MUH_GET_FIS_TURLERI_STRING(fis_turu);
        }
        else {
            fis_turu_str = MUH_GET_ENTEGRASYON_FIS_KAYNAK_MODUL_STRING(modul_id);
        }
        table_widget->item ( current_row,0 )->setText ( sql_query.VALUE(0).toString() );
        table_widget->item ( current_row,1 )->setText ( sql_query.VALUE("fis_no").toString() );
        table_widget->item ( current_row,2 )->setText ( sql_query.VALUE(1).toDate().toString("dd MMMM yyyy dddd") );
        table_widget->item ( current_row,3 )->setText ( (sql_query.VALUE(2).toString().replace("\n" , "")));
        table_widget->item ( current_row,4 )->setText ( VIRGUL_EKLE ( sql_query.VALUE(3).toString() ) );
        table_widget->item ( current_row,5 )->setText ( VIRGUL_EKLE ( sql_query.VALUE(4).toString() ) );
        table_widget->item ( current_row,6 )->setText ( fis_turu_str ) ;
    }


}

/**************************************************************************************
                   HATALI_FIS_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int HATALI_FIS_SECIMI::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();
    int fis_id = table_widget->item ( selected_row_number,0 )->text().toInt();

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("muh_fisler" , "fis_turu , modul_id" , "fis_id = :fis_id");

    query.SET_VALUE(":fis_id" , fis_id);

    int fis_turu    = 0;
    int fis_kaynagi = 0;

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        fis_turu    = query.VALUE(0).toInt();
        fis_kaynagi = query.VALUE(1).toInt();
    }

    OPEN_MUHASEBE_FISI ( fis_id , fis_turu , fis_kaynagi ,this );

    return ADAK_CONTINUE;
}

/**************************************************************************************
                   HATALI_FIS_SECIMI::FIS_HATALI_MI
***************************************************************************************/
bool HATALI_FIS_SECIMI::FIS_HATALI_MI(int fis_id, double toplam_borc, double toplam_alacak)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("muh_fis_satirlari" ,"SUM(borc_tutari),SUM(alacak_tutari)",
                         "fis_id = :fis_id");

    query.SET_VALUE(":fis_id" , fis_id);

    if ( query.SELECT() EQ 0 ) {
        return true;
    }

    query.NEXT();

    double fis_toplam_borc   = ROUND( query.VALUE(0).toDouble() );
    double fis_toplam_alacak = ROUND( query.VALUE(1).toDouble() );

    if ( fis_toplam_borc NE toplam_borc ) {
        return true;
    }
    if ( fis_toplam_alacak NE toplam_alacak ) {
        return true;
    }

    query.PREPARE_SELECT("muh_fis_satirlari" , "hesap_id" , "fis_id = :fis_id");

    query.SET_VALUE(":fis_id" , fis_id);

    if ( query.SELECT() EQ 0 ) {
        return true;
    }
    while ( query.NEXT() ) {

        if ( m_hesap_id_list.contains(query.VALUE(0).toString()) EQ false ) {
            return true;
        }
    }

    return false;
}

















