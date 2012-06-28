#include <QTableWidget>
#include "adak_sql.h"
#include "sube_depo_secimi_class.h"
#include "secim_kerneli.h"
#include "sube_console_utils.h"
#include "sube_depolari_formu_open.h"
#include "sube_enum.h"

extern ADAK_SQL *           DB;


/**************************************************************************************
                   SUBE_DEPO_SEC
***************************************************************************************/

int SUBE_DEPO_SEC ( int sube_id , QWidget * parent )
{
    int          secilen_id = -1;
    SUBE_DEPO_SECIMI * F = new SUBE_DEPO_SECIMI ( sube_id , parent );
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return ( secilen_id );
}

/**************************************************************************************
                   SUBE_DEPO_SECIMI::SUBE_DEPO_SECIMI
***************************************************************************************/

SUBE_DEPO_SECIMI::SUBE_DEPO_SECIMI( int sube_id , QWidget * parent ) : SECIM_KERNELI ( parent )
{

    m_sube_id = sube_id;

    SET_WINDOW_SIZE ( 300, 800 );
    SET_HEADERS (QStringList() << "depo id Id" << "Depo Kodu" << "Depo Adı" );

    SET_PAGE_TITLE    ( tr ( "ŞUBE - DEPO SEÇİMİ" ) );

    SET_HELP_PAGE     ( tr("sube_depo_secimi.html") );


    INIT_KERNEL ( DB );

    QTableWidget * tablewidget = GET_TABLE_WIDGET();

    tablewidget->hideColumn(0);
    tablewidget->setColumnWidth(1 , 40);
    tablewidget->setColumnWidth(2 , 60);

    SET_SETTING_NAME  ( "SUBE_DEPO_SECIMI" );
}

/**************************************************************************************
                   SUBE_DEPO_SECIMI::FILL_TABLE
***************************************************************************************/

void SUBE_DEPO_SECIMI::FILL_TABLE()
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    SQL_QUERY query(DB);

    query.PREPARE_SELECT ( "sub_depolar","depo_id, depo_kodu, depo_adi" );

    if ( SUBE_GET_DEPO_ISLEM_YETKISI() EQ SADECE_UNITE_DEPOSUNDA ) {
        query.AND_EKLE ( "sube_id = :sube_id" );
        query.SET_VALUE( ":sube_id" , m_sube_id);
    }

    if( query.SELECT() EQ 0 ) {
        return;
    }

    int current_row = 0;

    while ( query.NEXT()) {

        current_row = ADD_NEW_ROW();

        table_widget->item(current_row,0)->setText ( query.VALUE ( "depo_id" ).toString());
        table_widget->item(current_row,1)->setText ( query.VALUE ( "depo_kodu" ).toString());
        table_widget->item(current_row,2)->setText ( query.VALUE ( "depo_adi" ).toString());
    }
}

/**************************************************************************************
                   SUBE_DEPO_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int SUBE_DEPO_SECIMI::SINGLE_LINE_SELECTED(int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    *m_secilen_id = table_widget->item ( selected_row_number, 0 )->text().toInt();
    return ADAK_EXIT;
}
