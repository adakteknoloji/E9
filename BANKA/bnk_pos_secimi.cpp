#include "bnk_pos_secimi_class.h"
#include "bnk_pos_secimi_open.h"
#include "muh_console_utils.h"
#include "bnk_hesaplari_fisi_open.h"

extern ADAK_SQL * DB;

static int    g_hesap_no_id;

/*************************************************/
/*               OPEN_BNK_POS_SEC                */
/*************************************************/

int OPEN_BNK_POS_SEC (int hesap_no_id,QDialog * parent)
{
    int secilen_id    = -1;

    g_hesap_no_id = hesap_no_id;
    BNK_POS_SECIMI *F = new BNK_POS_SECIMI (parent);
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/*************************************************/
/*    BNK_POS_SECIMI::BNK_POS_SECIMI             */
/*************************************************/

BNK_POS_SECIMI::BNK_POS_SECIMI( QWidget *parent ) : SECIM_KERNELI(parent)
{
    SET_HELP_PAGE  ( "bnk_pos_secimi.html" );

    SET_WINDOW_SIZE(300,1100);
    SET_HEADERS(QStringList()<<"Pos Id"<<tr("Pos Numarası")<<tr("Üye İşyeri Adresi")<<tr("Telefonu")<<tr("Açıklama")
                <<tr("Muhasebe Hesabı")<<tr(" / Hesap İsmi")<<tr("Komisyon Hesabı")<<tr(" / Hesap İsmi"));

    SET_PAGE_TITLE( tr ( "BNK - POS SEÇİMİ" ) );

    INIT_KERNEL     ( DB );

    QTableWidget * table_widget = GET_TABLE_WIDGET ();
    table_widget->hideColumn     ( 0 );

    table_widget->setColumnWidth ( 1,130 );
    table_widget->setColumnWidth ( 2,130 );
    table_widget->setColumnWidth ( 3,100 );
    table_widget->setColumnWidth ( 4,150 );
    table_widget->setColumnWidth ( 5,130 );
    table_widget->setColumnWidth ( 6,140 );
    table_widget->setColumnWidth ( 7,120 );
    table_widget->setColumnWidth ( 8,120 );

    SET_SETTING_NAME              ( "BANKA_POS_SECIMI" );
}

/*************************************************/
/*         BNK_POS_SECIMI::FILL_TABLE            */
/*************************************************/

void BNK_POS_SECIMI::FILL_TABLE()
{
    QString where_str = "";

    if ( g_hesap_no_id > 0 ) {
        where_str += QString("hesap_no_id = %1").arg(g_hesap_no_id);
    }

    QTableWidget * table_widget = GET_TABLE_WIDGET();

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("bnk_hesabin_poslari","pos_id, pos_numarasi, uye_isyeri_adresi, uye_isyeri_telefonu, aciklama, "
                         "muh_hesap_id,kom_hesap_id",where_str);

    if( query.SELECT() EQ 0 ) {
        return;
    }
    int current_row;

    while (query.NEXT()) {

        current_row = ADD_NEW_ROW();

        table_widget->item(current_row,0)->setText(query.VALUE(0).toString());
        table_widget->item(current_row,1)->setText(query.VALUE(1).toString());
        table_widget->item(current_row,2)->setText(query.VALUE(2).toString());
        table_widget->item(current_row,3)->setText(query.VALUE(3).toString());
        table_widget->item(current_row,4)->setText(query.VALUE(4).toString());
        table_widget->item(current_row,5)->setText(MUH_GET_HESAP_KODU(query.VALUE(5).toInt()));
        table_widget->item(current_row,6)->setText(MUH_GET_HESAP_ISMI(query.VALUE(5).toInt()));
        table_widget->item(current_row,7)->setText(MUH_GET_HESAP_KODU(query.VALUE(6).toInt()));
        table_widget->item(current_row,8)->setText(MUH_GET_HESAP_ISMI(query.VALUE(6).toInt()));
    }
}

/*************************************************/
/*          BNK_POS_SECIMI::SINGLE_LINE_SELECTED       */
/*************************************************/

int BNK_POS_SECIMI::SINGLE_LINE_SELECTED(int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();
    *m_secilen_id = table_widget->item(selected_row_number,0)->text().toInt();
    return ADAK_EXIT;
}

