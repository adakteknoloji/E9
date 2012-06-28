#include "bnk_kredi_karti_secimi_class.h"
#include "bnk_kredi_karti_secimi_open.h"
#include "muh_console_utils.h"
#include "bnk_hesabin_kredi_kartlari_fisi_open.h"

extern ADAK_SQL * DB;

static int    g_hesap_no_id;

/*************************************************/
/*            OPEN_BNK_KREDI_KARTI_SEC              */
/*************************************************/

int OPEN_BNK_KREDI_KARTI_SEC (int hesap_no_id,QDialog * parent)
{
    int secilen_id    = -1;

    g_hesap_no_id = hesap_no_id;
    BNK_KREDI_KARTI_SECIMI * F = new BNK_KREDI_KARTI_SECIMI (parent);
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/*********************************************************/
/*    BNK_KREDI_KARTI_SECIMI::BNK_KREDI_KARTI_SECIMI     */
/*********************************************************/

BNK_KREDI_KARTI_SECIMI::BNK_KREDI_KARTI_SECIMI( QWidget *parent ) : SECIM_KERNELI(parent)
{
    SET_HELP_PAGE  ( "bnk_kredi_karti_secimi.html" );

    SET_WINDOW_SIZE(300,950);
    SET_HEADERS(QStringList()<<"KK Id"<<tr("Kredi Kartı Numarası")<<tr("Sahibi")<< tr("Açıklama")
                <<tr("Muhasebe Hesabı")<<tr("/ Hesap İsmi"));

    SET_PAGE_TITLE( tr ( "BNK - KREDİ KARTI SEÇİMİ" ) );
    INIT_KERNEL(DB);

    QTableWidget * table_widget = GET_TABLE_WIDGET();
    table_widget->hideColumn(0);

    table_widget->setColumnWidth    ( 1, 260 );
    table_widget->setColumnWidth    ( 2, 150 );
    table_widget->setColumnWidth    ( 3, 220 );
    table_widget->setColumnWidth    ( 4, 120 );
    table_widget->setColumnWidth    ( 5, 150 );

    SET_SETTING_NAME                ( "BANKA_KK_SECIMI" );
}

/*************************************************/
/*        BNK_KREDI_KARTI_SECIMI::FILL_TABLE     */
/*************************************************/

void BNK_KREDI_KARTI_SECIMI::FILL_TABLE()
{
    QString where_str = "";

    if (g_hesap_no_id > 0 ) {
        where_str += QString("hesap_no_id = %1").arg(g_hesap_no_id);
    }

    QTableWidget * table_widget = GET_TABLE_WIDGET();

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("bnk_hesabin_kredi_kartlari","kredi_karti_id,kredi_karti_numarasi,kredi_karti_sahibi,muh_hesap_id,aciklama",where_str);

    if( query.SELECT() EQ 0 ) {
        return;
    }
    int current_row;

    while (query.NEXT()) {

        current_row = ADD_NEW_ROW();

        table_widget->item(current_row,0)->setText(query.VALUE(0).toString());
        table_widget->item(current_row,1)->setText(query.VALUE(1).toString());
        table_widget->item(current_row,2)->setText(query.VALUE(2).toString());
        table_widget->item(current_row,3)->setText(query.VALUE(4).toString());
        table_widget->item(current_row,4)->setText(MUH_GET_HESAP_KODU(query.VALUE(3).toInt()));
        table_widget->item(current_row,5)->setText(MUH_GET_HESAP_ISMI(query.VALUE(3).toInt()));

    }

}

/*************************************************/
/* BNK_KREDI_KARTI_SECIMI::SINGLE_LINE_SELECTED  */
/*************************************************/

int BNK_KREDI_KARTI_SECIMI::SINGLE_LINE_SELECTED(int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    *m_secilen_id = table_widget->item(selected_row_number,0)->text().toInt();
    return ADAK_EXIT;
}

