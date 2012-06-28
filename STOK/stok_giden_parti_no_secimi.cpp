#include <QTableWidget>
#include <QStringList>
#include <QCheckBox>
#include "adak_sql.h"
#include "stok_giden_parti_no_secimi_class.h"
#include "stok_giden_parti_no_secimi_open.h"
#include "secim_kerneli.h"
#include "stok_console_utils.h"
#include "e9_console_utils.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   STOK_GIDEN_PARTI_NO_SEC
***************************************************************************************/

int STOK_GIDEN_PARTI_NO_SEC ( int p_depo_id, int p_urun_id, int p_secili_parti_no, QWidget * parent )
{
    int secilen_id;
    STOK_GIDEN_PARTI_NO_SECIMI * F = new STOK_GIDEN_PARTI_NO_SECIMI ( p_depo_id, p_urun_id, p_secili_parti_no, parent );
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );

    return secilen_id;
}

/**************************************************************************************
                   STOK_GIDEN_PARTI_NO_SECIMI::STOK_GIDEN_PARTI_NO_SECIMI
***************************************************************************************/

STOK_GIDEN_PARTI_NO_SECIMI::STOK_GIDEN_PARTI_NO_SECIMI( int p_depo_id, int p_urun_id, int p_secili_parti_no, QWidget * parent ) : SECIM_KERNELI ( parent )
{
    m_depo_id = p_depo_id;
    m_urun_id = p_urun_id;

    SET_SORTING(false);
    SET_WINDOW_SIZE ( 300, 700 );
    QString urun_adi;
    STK_GET_URUN_KODU_ADI( p_urun_id, NULL, &urun_adi );

    SET_HEADERS ( QStringList()<<tr("part_no_id")<<tr("Parti Kodu")<<tr("Ürün Miktarı") << tr("Üretim Tarihi") << tr("Son K. Tarihi"));

    SET_PAGE_TITLE    ( "PARTİ KODU SEÇİMİ" );
    SET_SETTING_NAME  ( "STOK_GIDEN_PARTI_NO_SECIMI" );
    SET_HELP_PAGE     ( tr("stok_giden_parti_no_secimi.html") );

    INIT_KERNEL ( DB );

    QTableWidget * tablewidget = GET_TABLE_WIDGET();

    tablewidget->hideColumn(0);

    tablewidget->setColumnWidth( 1 , 40);
    tablewidget->setColumnWidth( 2 , 20);
    tablewidget->setColumnWidth( 3 , 20);
    tablewidget->setColumnWidth( 4 , 20);

    QList< int > secili_olacak_id;
    secili_olacak_id << p_secili_parti_no;
    SELECT_LINES( secili_olacak_id );
}

/**************************************************************************************
                   STOK_GIDEN_PARTI_NO_SECIMI::FILL_TABLE
***************************************************************************************/

void STOK_GIDEN_PARTI_NO_SECIMI::FILL_TABLE()
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("stk_urunun_partileri",
                             "parti_id, parti_kodu, urun_uretim_tarihi, urun_son_kul_tarihi",
                             "urun_id = :urun_id AND depo_id = :depo_id" );

    sql_query.SET_VALUE(":urun_id", m_urun_id );
    sql_query.SET_VALUE(":depo_id", m_depo_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    int current_row = 0;

    while ( sql_query.NEXT() EQ true ) {

        int parti_id                = sql_query.VALUE(0).toInt();
        QString parti_kodu          = sql_query.VALUE(1).toString();
        QString urun_uretim_tarihi  = sql_query.VALUE(2).toString();
        QString urun_son_kul_tarihi = sql_query.VALUE(3).toString();

        double urun_miktari = ROUND ( STK_PARTIDEKI_URUN_MIKTARINI_BUL ( parti_id) );

        if( urun_miktari EQ 0.00 ) {
            continue;
        }

        current_row = ADD_NEW_ROW();

        table_widget->item ( current_row, 2 )->setTextAlignment ( Qt::AlignVCenter + Qt::AlignRight );
        table_widget->item ( current_row, 3 )->setTextAlignment ( Qt::AlignVCenter + Qt::AlignRight );
        table_widget->item ( current_row, 4 )->setTextAlignment ( Qt::AlignVCenter + Qt::AlignRight );

        table_widget->item ( current_row, 0 )->setText ( QString::number( parti_id )  );
        table_widget->item ( current_row, 1 )->setText ( parti_kodu );
        table_widget->item ( current_row, 2 )->setText ( VIRGUL_EKLE ( QVariant ( urun_miktari ).toString(), 2 ));
        table_widget->item ( current_row, 3 )->setText ( urun_uretim_tarihi );
        table_widget->item ( current_row, 4 )->setText ( urun_son_kul_tarihi );
    }

}

/**************************************************************************************
                   STOK_GIDEN_PARTI_NO_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int STOK_GIDEN_PARTI_NO_SECIMI::SINGLE_LINE_SELECTED(int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    *m_secilen_id = table_widget->item ( selected_row_number, 0 )->text().toInt();

    return ADAK_EXIT;
}
