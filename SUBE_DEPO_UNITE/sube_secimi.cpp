#include <QTableWidget>
#include "adak_sql.h"
#include "sube_secimi_class.h"
#include "secim_kerneli.h"
 

extern ADAK_SQL *           DB;


/**************************************************************************************
                   SUBE_SEC
***************************************************************************************/

int SUBE_SEC ( QWidget * parent )
{
    int          secilen_id = -1;
    SUBE_SECIMI * F = new SUBE_SECIMI ( parent );
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return ( secilen_id );
}

/**************************************************************************************
                   SUBE_SECIMI::SUBE_SECIMI
***************************************************************************************/

SUBE_SECIMI::SUBE_SECIMI( QWidget * parent ) : SECIM_KERNELI ( parent )
{

    SET_SORTING(false);
    SET_WINDOW_SIZE ( 300, 470 );
    SET_HEADERS (QStringList() << "Fis Id" << tr("Şube Kodu") << tr("Şube Adı"));

    SET_PAGE_TITLE    ( tr ( "ŞUBE - ŞUBE SEÇİMİ" ) );



    SET_HELP_PAGE     ( tr("sube_secimi.html") );

    INIT_KERNEL ( DB );

    QTableWidget * tablewidget = GET_TABLE_WIDGET();

    tablewidget->hideColumn(0);
    tablewidget->setColumnWidth(1 ,50);
    tablewidget->setColumnWidth(2 ,50);

    SET_SETTING_NAME  ( "SUBE_SECIMI" );
}

/**************************************************************************************
                   SUBE_SECIMI::FILL_TABLE
***************************************************************************************/

void SUBE_SECIMI::FILL_TABLE()
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("sub_subeler","sube_id,sube_kodu,sube_adi");

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    int current_row = 0;

    while ( query.NEXT() ) {
        current_row = ADD_NEW_ROW();

        table_widget->item(current_row,0)->setText(query.VALUE(0).toString());
        table_widget->item(current_row,1)->setText(query.VALUE(1).toString());
        table_widget->item(current_row,2)->setText(query.VALUE(2).toString());
    }


}

/**************************************************************************************
                   SUBE_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int SUBE_SECIMI::SINGLE_LINE_SELECTED(int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    *m_secilen_id = table_widget->item ( selected_row_number, 0 )->text().toInt();
    return ADAK_EXIT;
}


