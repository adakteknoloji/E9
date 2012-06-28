#include "dmr_grup_tree_secimi_class.h"
#include "adak_gruplar.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   SHOW_DMR_GRUP_TREE_SECIMI
***************************************************************************************/

int SHOW_DMR_GRUP_TREE_SECIMI (QWidget * parent)
{
    int secilen_id = -1;

    DMR_GRUP_TREE_SECIMI * F = new DMR_GRUP_TREE_SECIMI (parent);
    F->m_secilen_id =&secilen_id ;
    F->EXEC( NOT_FULL_SCREEN );

    return secilen_id;
}

/**************************************************************************************
                   DMR_GRUP_TREE_SECIMI::DMR_GRUP_TREE_SECIMI
***************************************************************************************/

DMR_GRUP_TREE_SECIMI::DMR_GRUP_TREE_SECIMI ( QWidget * parent ) : TREE_SECIM_KERNEL (parent)
{
    SET_HELP_PAGE       ( "sabit-kiymetler-demirbaslar_rapor-islem-gruplari" );

    SET_SETTING_NAME    ( "DMR_GRUP_TREE_SECIMI" );

    SET_WINDOW_SIZE    (200,500);

    SET_COLUMN_HEADERS ( QStringList()<<tr("grup_id")<<("Grup Kodu")<<tr("Grup İsmi")<<("Alt Grup Sayısı")<<("Gruptaki Kayıt Sayısı") );

    SET_PAGE_TITLE( tr("DMR - DEMİRBAŞ GRUP SEÇİMİ") );


    INIT_KERNEL ( this, DB );

    m_tree_widget = GET_TREE_VIEW();

    HIDE_COLUMN(0);

    m_tree_widget->setColumnWidth( 1, 80  );
    m_tree_widget->setColumnWidth( 2, 300 );
    m_tree_widget->setColumnWidth( 3, 100 );
    m_tree_widget->setColumnWidth( 4, 100 );

}

/**************************************************************************************
                   DMR_GRUP_TREE_SECIMI::~DMR_GRUP_TREE_SECIMI
***************************************************************************************/

DMR_GRUP_TREE_SECIMI::~DMR_GRUP_TREE_SECIMI()
{

}

/**************************************************************************************
                   DMR_GRUP_TREE_SECIMI::FILL_TREE_TABLE
***************************************************************************************/

void DMR_GRUP_TREE_SECIMI::FILL_TREE_TABLE()
{
    SQL_QUERY     sql_query          ( DB );

    //parent itemlarimizi query ile buluyoruz.
    sql_query.PREPARE_SELECT ("adak_gruplar" ,"grup_id,grup_parent_id,grup_seviyesi,grup_kodu, "
                              "grup_adi,tam_grup_kodu,grup_son_seviye_mi,gruptaki_kayit_sayisi, "
                              "alt_grup_sayisi","modul_id=:modul_id AND program_id=:program_id" );

    sql_query.SET_VALUE      (":program_id",E9_PROGRAMI);
    sql_query.SET_VALUE      (":modul_id"  ,DEMIRBAS_MODULU);

    if ( sql_query.SELECT() NE 0 ) {

        while ( sql_query.NEXT() EQ true ) {

           int grup_id = sql_query.VALUE(0).toInt();

           QStringList tree_row;
                       tree_row << QVariant( grup_id ).toString() << sql_query.VALUE("grup_kodu").toString()
                                << sql_query.VALUE("grup_adi" ).toString() << sql_query.VALUE("alt_grup_sayisi").toString()
                                << sql_query.VALUE("gruptaki_kayit_sayisi").toString();

           //ve bu row u tree ye ekleriz.
           ADD_PARENT_ITEM ( tree_row , sql_query.VALUE("alt_grup_sayisi").toInt());
       }
    }
}

/**************************************************************************************
                   DMR_GRUP_TREE_SECIMI::ADD_CHILD_ITEMS
***************************************************************************************/

void DMR_GRUP_TREE_SECIMI::ADD_CHILD_ITEMS(QStringList column_datas)
{
    SQL_QUERY     sql_query          ( DB );

    int grup_parent_id = column_datas.at(0).toInt();

    //child itemlarimizi query ile buluyoruz.
    sql_query.PREPARE_SELECT ("adak_gruplar" ,"grup_id,grup_parent_id,grup_seviyesi,grup_kodu, "
                              "grup_adi,tam_grup_kodu,grup_son_seviye_mi,gruptaki_kayit_sayisi, "
                              "alt_grup_sayisi,modul_id,program_id","grup_parent_id=:grup_parent_id AND "
                              "modul_id=:modul_id AND program_id=:program_id" );

    sql_query.SET_VALUE      (":program_id",E9_PROGRAMI);
    sql_query.SET_VALUE      (":modul_id"  ,DEMIRBAS_MODULU);

    sql_query.SET_VALUE(":grup_parent_id",grup_parent_id);

    if ( sql_query.SELECT() NE 0 ) {

        while ( sql_query.NEXT() EQ true ) {

           int grup_id = sql_query.VALUE(0).toInt();

           QStringList tree_row;
                       tree_row << QVariant( grup_id ).toString() << sql_query.VALUE("grup_kodu").toString()
                                << sql_query.VALUE("grup_adi" ).toString() << sql_query.VALUE("alt_grup_sayisi").toString()
                                << sql_query.VALUE("gruptaki_kayit_sayisi").toString();

           //ve bu row u tree ye ekleriz.
           ADD_CHILD_ITEM( tree_row , sql_query.VALUE("alt_grup_sayisi").toInt());
       }
    }
}

/**************************************************************************************
                   DMR_GRUP_TREE_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int DMR_GRUP_TREE_SECIMI::SINGLE_LINE_SELECTED(QStringList column_datas)
{
    *m_secilen_id = column_datas.at(0).toInt();

    return ADAK_EXIT;
}

