#include <QTableWidget>
#include <QStringList>
#include <QCheckBox>
#include "adak_sql.h"
#include "stok_giden_seri_no_secimi_class.h"
#include "stok_giden_seri_no_secimi_open.h"
#include "secim_kerneli.h"
#include "stok_console_utils.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   STOK_GIDEN_SERI_NO_SEC
***************************************************************************************/

void STOK_GIDEN_SERI_NO_SEC ( int p_depo_id, int p_urun_id, QStringList *p_seri_no_list, URUN_DEPO_DURMU p_urun_durumu,
                              bool p_sadece_liste, int p_fatura_satiri_id, QWidget * parent )
{
    STOK_GIDEN_SERI_NO_SECIMI * F = new STOK_GIDEN_SERI_NO_SECIMI ( p_depo_id, p_urun_id, p_seri_no_list, p_urun_durumu, p_sadece_liste, p_fatura_satiri_id, parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   STOK_GIDEN_SERI_NO_SECIMI::STOK_GIDEN_SERI_NO_SECIMI
***************************************************************************************/

STOK_GIDEN_SERI_NO_SECIMI::STOK_GIDEN_SERI_NO_SECIMI( int p_depo_id, int p_urun_id, QStringList *p_seri_no_list, URUN_DEPO_DURMU p_urun_durumu,
                                                      bool p_sadece_liste, int p_fatura_satiri_id, QWidget * parent ) : SECIM_KERNELI ( parent )
{
    m_depo_id       = p_depo_id;
    m_urun_id       = p_urun_id;
    m_urun_durumu   = p_urun_durumu;
    m_sadece_liste  = p_sadece_liste;
    m_fatura_satiri_id = p_fatura_satiri_id;

    m_secilen_seri_nolar = p_seri_no_list;

    SET_SORTING(false);
    SET_WINDOW_SIZE ( 300, 500 );
    QString urun_adi;
    STK_GET_URUN_KODU_ADI( p_urun_id, NULL, &urun_adi );

    SET_HEADERS ( QStringList() << "Seri Numarası" );

    SET_PAGE_TITLE    ( "SERİ NO SEÇİMİ" );
    SET_SETTING_NAME  ( "STOK_GIDEN_SERI_NO_SECIMI" );
    SET_HELP_PAGE     ( tr("stok_giden_seri_no_secimi.html") );

    ENABLE_MULTI_ROW_SELECTION();

    INIT_KERNEL ( DB );

    QTableWidget * tablewidget = GET_TABLE_WIDGET();

    tablewidget->setColumnWidth( 0 , 80);
    tablewidget->setColumnWidth( 1 , 10);


    SELECT_LINES( m_secili_olacak_satirlar );

}

/**************************************************************************************
                   STOK_GIDEN_SERI_NO_SECIMI::FILL_TABLE
***************************************************************************************/

void STOK_GIDEN_SERI_NO_SECIMI::FILL_TABLE()
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    for (int i = 0; i < m_secilen_seri_nolar->size(); ++i) {
        int current_row = ADD_NEW_ROW();
        table_widget->item( current_row, 0)->setText ( m_secilen_seri_nolar->at( i ) );

        table_widget->selectRow( current_row );

        m_secili_olacak_satirlar.append( current_row );
    }

    if ( m_sadece_liste EQ false ) {
        SQL_QUERY query(DB);

        query.PREPARE_SELECT ( "stk_urunun_seri_nolari","seri_numarasi",
                               "depo_id = :depo_id AND urun_id = :urun_id" );

        query.SET_VALUE(":depo_id"       , m_depo_id );
        query.SET_VALUE(":urun_id"       , m_urun_id );

        if ( m_urun_durumu EQ ENUM_URUN_DEPODA ) {
            query.AND_EKLE( "depoda_mi = :depoda_mi");
            query.SET_VALUE(":depoda_mi" , 1  );
        }
        else if ( m_urun_durumu EQ ENUM_URUN_DEPODA_DEGIL ) {
            query.AND_EKLE( "depoda_mi = :depoda_mi");
            query.SET_VALUE(":depoda_mi" , 0  );
        }

        if ( m_fatura_satiri_id NE -1 ) {
            query.AND_EKLE( "fatura_satiri_id = :fatura_satiri_id");
            query.SET_VALUE(":fatura_satiri_id" , m_fatura_satiri_id  );
        }

        if( query.SELECT( "seri_numarasi ASC") EQ 0 ) {
            return;
        }

        while (query.NEXT()) {

            QString seri_numarasi = query.VALUE ( 0 ).toString();

            if ( m_secilen_seri_nolar->contains( seri_numarasi ) EQ false ) {
                int current_row = ADD_NEW_ROW();
                table_widget->item( current_row, 0 )->setText ( seri_numarasi );
            }
        }
    }
}

/**************************************************************************************
                   STOK_GIDEN_SERI_NO_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int STOK_GIDEN_SERI_NO_SECIMI::SINGLE_LINE_SELECTED(int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();
    m_secilen_seri_nolar->clear();
    m_secilen_seri_nolar->append( table_widget->item ( selected_row_number, 0 )->text() );

    return ADAK_EXIT;
}

/**************************************************************************************
                   STOK_GIDEN_SERI_NO_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int STOK_GIDEN_SERI_NO_SECIMI::MULTI_LINE_SELECTED( QList<int> selected_rows )
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();
    m_secilen_seri_nolar->clear();
    for (int i = 0; i < selected_rows.size(); i++ ) {
        m_secilen_seri_nolar->append( table_widget->item ( selected_rows.at( i ), 0 )->text() );
    }

    return ADAK_EXIT;
}
