#include "adak_sql.h"
#include "bnk_makro_secimi_class.h"
#include "bnk_makro_secimi_open.h"
 

extern ADAK_SQL * DB;

/**************************************************************************************
                   BANKA_MAKRO_SEC
***************************************************************************************/

int OPEN_BNK_MAKRO_SEC ( QWidget * parent )
{
    int secilen_id    = -1;

    BNK_MAKRO_SECIMI *F = new BNK_MAKRO_SECIMI ( parent );
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/**************************************************************************************
                   BNK_MAKRO_SECIMI::BNK_MAKRO_SECIMI
***************************************************************************************/

BNK_MAKRO_SECIMI::BNK_MAKRO_SECIMI ( QWidget *parent ) : SECIM_KERNELI ( parent )
{
    SET_HELP_PAGE  ( "bnk_islem_secimi.html" );

    SET_WINDOW_SIZE     ( 300,800 );
    SET_HEADERS         ( QStringList()<<"banka_makro_id"<<"İşlem Adı" );

    SET_PAGE_TITLE      ( tr ( "BNK - BANKA İŞLEM SEÇİMİ" ) );

    INIT_KERNEL         ( DB );

    QTableWidget * table_widget = GET_TABLE_WIDGET();

    table_widget->hideColumn ( 0 );
    table_widget->setColumnWidth( 1, 100 );


    SET_SETTING_NAME    ( "BANKA_HESAPLARI_SECIMI" );

}

/**************************************************************************************
                   BNK_MAKRO_SECIMI::FILL_TABLE
***************************************************************************************/

void BNK_MAKRO_SECIMI::FILL_TABLE()
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "bnk_makrolar","makro_id,makro_adi" );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    int current_row = 0;

    while ( query.NEXT () ) {
        current_row = ADD_NEW_ROW();

        table_widget->item ( current_row,0 )->setText ( query.VALUE ( "makro_id" ).toString() );
        table_widget->item ( current_row,1 )->setText ( query.VALUE ( "makro_adi" ).toString() );
    }
}

/**************************************************************************************
                   BNK_MAKRO_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int BNK_MAKRO_SECIMI::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    *m_secilen_id = table_widget->item ( selected_row_number,0 )->text().toInt();

    return ADAK_EXIT;
}
